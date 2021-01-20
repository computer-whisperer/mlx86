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
#include "solvers/solvers.h"
#include "types.h"
#include "utils.h"

#define NUM_PROCESSES 10


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

const struct ParallelTempering_Hyperparameters_T parallel_tempering_default_hyperparameters = {
	1000000, // U32 cycle_len;
	10, // U32 recheck_rate;
	10000.0, // F64 score_diff_multiplier;
	10, // U32 neighbor_post_rate;
	100, // U32 neighbor_poll_rate;
	10, // U32 neighbor_poll_chance;
	10000.0, //F64 score_diff_neighbor_multiplier
	10 // U32 num_neighbors;
};

void tempering_process(struct Problem_T * problem, const struct ParallelTempering_Hyperparameters_T * hyperparameters, struct sync_mem_t * our_sync, struct sync_mem_t ** neighbor_syncs, U8* should_exit, struct reporting_mem_t * reporting_mem)
{
  if (problem->trial_initializer)
  {
    problem->trial_initializer(problem);
  }
  
	U8 * data = malloc(problem->data_len);
	U8 * data_b = malloc(problem->data_len);
	problem->data_initializer(problem, data);
	memcpy(data_b, data, problem->data_len);

	float current_score_total = problem->scalar_trial(problem, data);
	int current_score_count = 1;
	float current_score = current_score_total/current_score_count;

	U64 total_iterations = 0;

	U32 i = fast_rand()%hyperparameters->cycle_len;

	while(!(*should_exit))
	{
	    while (i < hyperparameters->cycle_len)
	    {
        if (*should_exit)
        {
          break;
        }
	    	// Copy data to data_b and modify it
	    	memcpy(data_b, data, problem->data_len);

	    	if ((i%10) == 0)
	    	{
	    		//Chip in to total
	    		sem_wait(&(reporting_mem->sem));
	    		reporting_mem->tests_run += 10;
	    		sem_post(&(reporting_mem->sem));
	    	}
		    if ((i%hyperparameters->recheck_rate) == 0)
		    {
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
	    		float t = (float)hyperparameters->cycle_len/(float)(i);
	    		float p = exp((float)((new_score - current_score)*hyperparameters->score_diff_multiplier)/t);
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

	    	if ((i%hyperparameters->neighbor_post_rate) == 0)
	    	{

	    		// Save ours to the sync
	    		sem_wait(&(our_sync->sem));
	    		our_sync->score = current_score;
	    		memcpy(our_sync->data, data, problem->data_len);
	    		sem_post(&(our_sync->sem));
	    	}

	    	if (((i%hyperparameters->neighbor_poll_rate) == 0) && ((fast_rand()%hyperparameters->neighbor_poll_chance) == 0))
	    	{
				// Sync with neighbors
	    		for (U8 j = 0; j < hyperparameters->num_neighbors; j++)
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
              float t = (float)hyperparameters->cycle_len/(float)(i);
              float p = exp((float)((current_score - neighbor_syncs[j]->score)*hyperparameters->score_diff_neighbor_multiplier)/t);
              does_pass = fast_rand() > (32767.0 * p);
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
  if (problem->trial_deinitializer)
  {
    problem->trial_deinitializer(problem);
  }
}


struct shared_data_t
{
	U8 should_exit;
	struct reporting_mem_t reporting_mem;
	struct sync_mem_t sync_mems[NUM_PROCESSES];
};

void parallel_tempering(struct Problem_T * problem, const struct ParallelTempering_Hyperparameters_T * hyperparameters, U8 reporting, double score_limit, U32 trial_limit, U8 * data_out, double * score_out, U32 * iterations_out)
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
			set_process_offset(i+1);
			struct sync_mem_t * neighbors[hyperparameters->num_neighbors];
			for (U32 j = 0; j <  hyperparameters->num_neighbors; j++)
			{
				neighbors[j] = &(shared_data->sync_mems[fast_rand()%NUM_PROCESSES]);
			}
			tempering_process(problem, hyperparameters, &(shared_data->sync_mems[i]), neighbors, &(shared_data->should_exit), &(shared_data->reporting_mem));
			exit(0);
		}
	}


	if (reporting)
	{
    if (problem->trial_initializer)
    {
      problem->trial_initializer(problem);
    }
    
		// Reporting process
		double start_time = getUnixTime();

		double prev_time = getUnixTime();
		U64 prev_tests_run = 0;
		while(1)
		{
			sleep(1);
			sem_wait(&(shared_data->sync_mems[0].sem));
			sem_wait(&(shared_data->reporting_mem.sem));

			double curr_time = getUnixTime();


			putchar('\n');
			printf("Time: %d\n", (U32)(curr_time - start_time));

			printf("Test rate: %f\n", (shared_data->reporting_mem.tests_run - prev_tests_run)/(curr_time - prev_time));
			printf("Games played: %d\n", shared_data->reporting_mem.tests_run);

		    printf("Score: %f\n", shared_data->sync_mems[0].score);
		    printf("Data:\n");
		    problem->data_pretty_printer(problem, shared_data->sync_mems[0].data);
		    if (shared_data->sync_mems[0].score >= score_limit)
		    {
		    	break;
		    }
			prev_time = curr_time;
			prev_tests_run = shared_data->reporting_mem.tests_run;

			sem_post(&(shared_data->sync_mems[0].sem));
			sem_post(&(shared_data->reporting_mem.sem));
			putchar('\n');


		}
    if (problem->trial_deinitializer)
    {
      problem->trial_deinitializer(problem);
    }
		printf("Finished after %f seconds.\n", (getUnixTime() - start_time));
	}
	else
	{
		// Wait for conditions to be met
		while (1)
		{
			usleep(1000);

			sem_wait(&(shared_data->sync_mems[0].sem));
			sem_wait(&(shared_data->reporting_mem.sem));
			double score = shared_data->sync_mems[0].score;
			U64 tests_run = shared_data->reporting_mem.tests_run;
			sem_post(&(shared_data->sync_mems[0].sem));
			sem_post(&(shared_data->reporting_mem.sem));
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

	// Close all threads
	shared_data->should_exit = 1;
	// Wait for subprocesses
	for (int i = 0; i < NUM_PROCESSES; i++)
	{
		waitpid(tempering_pids[i], NULL, 0);
	}

  if (data_out)
  {
    memcpy(data_out, shared_data->sync_mems[0].data, problem->data_len);
  }
  if (score_out)
  {
    *score_out = shared_data->sync_mems[0].score;
  }
	if (iterations_out)
  {
    *iterations_out = shared_data->reporting_mem.tests_run;
  }
}
