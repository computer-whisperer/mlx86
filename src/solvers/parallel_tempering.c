#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#include "problems/problems.h"
#include "types.h"
#include "utils.h"

#define NUM_PROCESSES 10
#define NUM_NEIGHBORS 3

#define CYCLE_LEN 100000

struct reporting_mem_t
{
	sem_t sem;
	U64 tests_run;
};

struct sync_mem_t
{
	sem_t sem;
	float score;
	U8 data[0x1000];
};

static volatile int keepRunning = 1;

void intHandler(int dummy) {
    keepRunning = 0;
}


void status_reporting_process(struct Problem_T * problem, struct reporting_mem_t * reporting_mem, struct sync_mem_t * sync_mem) {
	double start_time = getUnixTime();

	double prev_time = getUnixTime();
	U64 prev_tests_run = 0;

	signal(SIGINT, intHandler);
	while(keepRunning)
	{
		sleep(1);
		sem_wait(&(sync_mem->sem));
		sem_wait(&(reporting_mem->sem));

		double curr_time = getUnixTime();


		putchar('\n');
		printf("Time: %d\n", (U32)(curr_time - start_time));

		printf("Test rate: %f\n", (reporting_mem->tests_run - prev_tests_run)/(curr_time - prev_time));
		printf("Games played: %d\n", reporting_mem->tests_run);

	    printf("Score: %f\n", sync_mem->score);
	    printf("Data:\n");
	    problem->data_pretty_printer(problem, sync_mem->data);
	    if (sync_mem->score > 0.99999)
	    {
	    	keepRunning = 0;
	    }
		prev_time = curr_time;
		prev_tests_run = reporting_mem->tests_run;

		sem_post(&(sync_mem->sem));
		sem_post(&(reporting_mem->sem));
		putchar('\n');


	}
	printf("Finished after %f seconds.\n", (getUnixTime() - start_time));
}


void tempering_process(struct Problem_T * problem, struct sync_mem_t * our_sync, struct sync_mem_t * neighbor_syncs[NUM_NEIGHBORS], U8* should_exit, struct reporting_mem_t * reporting_mem)
{
	U8 * data = malloc(problem->data_len);
	U8 * data_b = malloc(problem->data_len);
	problem->data_initializer(problem, data);
	memcpy(data_b, data, problem->data_len);

	float current_score_total = problem->scalar_trial(problem, data);
	int current_score_count = 1;
	float current_score = current_score_total/current_score_count;

	U64 total_iterations = 0;

	U32 i = fast_rand()%CYCLE_LEN;

	while(!(*should_exit))
	{
	    while (i < CYCLE_LEN)
	    {
	    	// Copy data to data_b and modify it
	    	memcpy(data_b, data, problem->data_len);

	    	if ((i%10) == 0)
	    	{
	    		//Chip in to total
	    		sem_wait(&(reporting_mem->sem));
	    		reporting_mem->tests_run += 10;
	    		sem_post(&(reporting_mem->sem));

				// Average prev score with new run
				current_score_total += problem->scalar_trial(problem, data);
				current_score_count += 1;
				current_score = current_score_total/current_score_count;
	    	}

			/* Make a random change and introduce a new agent */
	    	problem->scrambler(problem, data);

	    	// Score run
	    	float new_score = problem->scalar_trial(problem, data);

	    	// Simulated annealing to determine if this passes
	    	int does_pass = 0;
	    	if (new_score >= current_score)
	    	{
	    		does_pass = 1;
	    	}
	    	else {
	    		float t = (float)CYCLE_LEN/(float)(i);
	    		float p = exp((float)((new_score - current_score)*10000.0)/t);
	        	does_pass = fast_rand() < (32767.0 * p);
	        }
		    if (does_pass) {
				// Keep the new code
				current_score = new_score;
				current_score_total = new_score;
				current_score_count = 1;
	        }
		    else
		    {
				// Revert to the old code
				U8* t = data_b;
				data_b = data;
				data = t;
	        }

	    	if ((i%100) == 0)
	    	{

	    		// Save ours to the sync
	    		sem_wait(&(our_sync->sem));
	    		our_sync->score = current_score;
	    		memcpy(our_sync->data, data, problem->data_len);
	    		sem_post(&(our_sync->sem));
	    	}

	    	if (((i%1000) == 0) && (fast_rand()%10 == 0))
	    	{
				// Sync with neighbors
	    		for (U8 j = 0; j < NUM_NEIGHBORS; j++)
	    		{
	    			sem_wait(&(neighbor_syncs[j]->sem));
	    			U8 does_pass = 0;
	    			float err = neighbor_syncs[j]->score - current_score;
            if (err < 0)
	    			{
	    				does_pass = 0;
	    			}
            else
            {
              //float t = (float)CYCLE_LEN/(float)(i);
              //float p = exp((float)((current_score - neighbor_syncs[j]->score)*10000.0)/t);
              //does_pass = fast_rand() > (32767.0 * p);
              does_pass = 1;
            }
            if (does_pass) {
	    				// Switch ours with the neighbor's
	    				current_score_total = neighbor_syncs[j]->score;
	    				current_score_count = 1;
	    				current_score = current_score_total/current_score_count;
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
}


struct shared_data_t
{
	U8 should_exit;
	struct reporting_mem_t reporting_mem;
	struct sync_mem_t sync_mems[NUM_PROCESSES];
};

U8 * parallel_tempering(struct Problem_T * problem)
{
	// Setup process data
	struct shared_data_t * shared_data = mmap(NULL, sizeof(struct shared_data_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

	shared_data->should_exit = 0;
	sem_init(&(shared_data->reporting_mem.sem), 1, 1);
	shared_data->reporting_mem.tests_run = 0;
	for (U32 i = 0; i < NUM_PROCESSES; i++)
	{
		sem_init(&(shared_data->sync_mems[i].sem), 1, 1);
		problem->data_initializer(problem, shared_data->sync_mems[i].data);
		shared_data->sync_mems[i].score = 0;
	}

	// Fork processes
	pid_t tempering_pids[NUM_PROCESSES];
	for (U32 i = 0; i < NUM_PROCESSES; i++)
	{
		tempering_pids[i] = fork();
		if (tempering_pids[i] == 0)
		{
			// Nudge rng
			seed_fast_rand(i);
			set_process_offset(i);
			struct sync_mem_t * neighbors[NUM_NEIGHBORS];
			for (U32 j = 0; j <  NUM_NEIGHBORS; j++)
			{
				neighbors[j] = &(shared_data->sync_mems[fast_rand()%NUM_PROCESSES]);
			}
			tempering_process(problem, &(shared_data->sync_mems[i]), neighbors, &(shared_data->should_exit), &(shared_data->reporting_mem));
			exit(0);
		}
	}
	// Reporting process
	status_reporting_process(problem, &(shared_data->reporting_mem), &(shared_data->sync_mems[0]));
	// Close all threads
	shared_data->should_exit = 1;
	// Wait for subprocesses
	for (int i = 0; i < NUM_PROCESSES; i++)
	{
		waitpid(tempering_pids[i], NULL, 0);
	}

	U8 * ret = malloc(problem->data_len);
	memcpy(ret, shared_data->sync_mems[0].data, problem->data_len);
	return ret;
}
