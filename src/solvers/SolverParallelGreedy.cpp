#include <semaphore.h>
#include <sys/mman.h>
#include <unistd.h>
#include <christian_utils.h>
#include <thread>
#include <vector>
#include <cstring>
#include "problems/problem.h"
#include "solvers/Solver.h"
#include "types.h"
#include "SolverParallelGreedy.h"

using namespace std;

#define NUM_PROCESSES 40

struct reporting_mem_t
{
	sem_t sem;
	U64 tests_run;
};

struct sync_mem_t
{
	sem_t sem;
	float score;
	U8 data[MAX_DATA_LEN];
};

void SolverParallelGreedy::tempering_process(Problem *problem_in, struct sync_mem_t *our_sync,
                                             sync_mem_t **neighbor_syncs, U8 *should_exit,
                                             struct reporting_mem_t *reporting_mem) {

    Problem * problem = problem_in->Clone();

    U8 *data = static_cast<U8 *>(malloc(problem->data_len));
    U8 *data_b = static_cast<U8 *>(malloc(problem->data_len));
    problem->dataInit(data);
    memcpy(data_b, data, problem->data_len);

    float current_score_total = problem->scalarTrial(data);
    int current_score_count = 1;
    float current_score = current_score_total / current_score_count;

    U64 total_iterations = 0;

    U32 i = fast_rand() % cycle_len;

    while (!(*should_exit)) {
        while (i < cycle_len) {
            if (*should_exit) {
                break;
            }
            // Copy data to data_b and modify it
            memcpy(data_b, data, problem->data_len);

            if ((i % 10) == 0) {
                //Chip in to total
                sem_wait(&(reporting_mem->sem));
                reporting_mem->tests_run += 10;
                sem_post(&(reporting_mem->sem));
            }
            if ((i % recheck_rate) == 0) {
                // Average prev score with new run
                current_score_total += problem->scalarTrial(data);
                current_score_count += 1;
                current_score = current_score_total / current_score_count;
            }

            /* Make a random change and introduce a new agent */
            problem->scrambler(data);

            // Score run
            float new_score = problem->scalarTrial(data);

            // Greedy step to determine if this passes
            if (new_score >= current_score) {
                // Keep the new code
                current_score = new_score;
                current_score_total = new_score;
                current_score_count = 1;
            } else {
                // Revert to the old code
                U8 *t = data_b;
                data_b = data;
                data = t;
            }

            if ((i % neighbor_post_rate) == 0) {

                // Save ours to the sync
                sem_wait(&(our_sync->sem));
                our_sync->score = current_score;
                memcpy(our_sync->data, data, problem->data_len);
                sem_post(&(our_sync->sem));
            }

            if (((i % neighbor_poll_rate) == 0) && ((fast_rand() % neighbor_poll_chance) == 0)) {
                // Sync with neighbors
                for (U8 j = 0; j < num_neighbors; j++) {
                    sem_wait(&(neighbor_syncs[j]->sem));
                    U8 does_pass = 0;
                    float err = neighbor_syncs[j]->score - current_score;
                    if (neighbor_syncs[j]->score > current_score) {
                        // Switch ours with the neighbor's
                        current_score_total = neighbor_syncs[j]->score;
                        current_score_count = 1;
                        current_score = current_score_total / current_score_count;
                        memcpy(data, neighbor_syncs[j]->data, problem->data_len);
                    }
                    sem_post(&(neighbor_syncs[j]->sem));
                }
            }

            total_iterations++;
            i++;
        }
        i = 0;
    }
    delete problem;
}


struct shared_data_t
{
	U8 should_exit;
	struct reporting_mem_t reporting_mem;
	struct sync_mem_t sync_mems[NUM_PROCESSES];
};

void SolverParallelGreedy::run(Problem *problem, struct REPORTER_MEM_T * reporter_mem, double score_limit, U32 trial_limit, struct SolverResults_T * results_out, uint8_t* starting_data)
{
	// Setup process data
	struct shared_data_t * shared_data = static_cast<shared_data_t *>(mmap(NULL, sizeof(struct shared_data_t),
                                                                           PROT_READ | PROT_WRITE,
                                                                           MAP_SHARED | MAP_ANONYMOUS, -1, 0));

	shared_data->should_exit = 0;
	sem_init(&(shared_data->reporting_mem.sem), 1, 1);
	shared_data->reporting_mem.tests_run = 0;
	for (U32 i = 0; i < NUM_PROCESSES; i++)
	{
		sem_init(&(shared_data->sync_mems[i].sem), 1, 1);
    if (starting_data)
    {
      memcpy(shared_data->sync_mems[i].data, starting_data, problem->data_len);
    }
    else
    {
      problem->dataInit(shared_data->sync_mems[i].data);
    }
		shared_data->sync_mems[i].score = 0;
	}

	// Fork processes
	vector<thread> threads;
	for (U32 i = 0; i < NUM_PROCESSES; i++)
	{
	    threads.emplace_back([this, i, shared_data, problem] {
		{
			// Nudge rng
			seed_fast_rand(i);
			set_process_offset(i+1);
			struct sync_mem_t * neighbors[num_neighbors];
			for (U32 j = 0; j <  num_neighbors; j++)
			{
				neighbors[j] = &(shared_data->sync_mems[fast_rand()%NUM_PROCESSES]);
			}
			tempering_process(problem, &(shared_data->sync_mems[i]), neighbors, &(shared_data->should_exit), &(shared_data->reporting_mem));
		}});
	}

	if (reporter_mem)
	{
    
		// Reporting process
		while(!reporter_mem->abort_solve)
		{
			usleep(100000);
			reporter_mem->mtx.lock();

			sem_wait(&(shared_data->sync_mems[0].sem));
			double score = shared_data->sync_mems[0].score;
			memcpy(reporter_mem->current_data, shared_data->sync_mems[0].data, problem->data_len);
			sem_post(&(shared_data->sync_mems[0].sem));

			sem_wait(&(shared_data->reporting_mem.sem));
			U64 tests_run = shared_data->reporting_mem.tests_run;
			sem_post(&(shared_data->reporting_mem.sem));


			reporter_mem->trials_completed = tests_run;
			reporter_mem->mtx.unlock();

	    	if (score >= score_limit)
	    	{
	    		break;
	    	}

	    	if (tests_run >= trial_limit)
	    	{
	    		break;
	    	}
		}
	}
	else
	{
		// Wait for conditions to be met
		while (1)
		{
			usleep(1000);

			sem_wait(&(shared_data->sync_mems[0].sem));
			double score = shared_data->sync_mems[0].score;
			sem_post(&(shared_data->sync_mems[0].sem));
	    	if (score >= score_limit)
	    	{
	    		break;
	    	}


			sem_wait(&(shared_data->reporting_mem.sem));
			U64 tests_run = shared_data->reporting_mem.tests_run;
			sem_post(&(shared_data->reporting_mem.sem));
	    	if (tests_run >= trial_limit)
	    	{
	    		break;
	    	}
		}
	}



	// Close all threads
	shared_data->should_exit = 1;
	// Wait for subprocesses

	for (auto &it: threads)
	{
	    it.join();
	}

	if (results_out)
	{
		results_out->data = static_cast<U8 *>(malloc(problem->data_len));
		results_out->score = shared_data->sync_mems[0].score;
		results_out->trial_count = shared_data->reporting_mem.tests_run;
		memcpy(results_out->data, shared_data->sync_mems[0].data, problem->data_len);
	}
}