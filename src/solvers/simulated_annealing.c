/*
 * simulated_annealing.c
 *
 *  Created on: Nov 16, 2020
 *      Author: christian
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "solvers/solvers.h"
#include "problems/problems.h"
#include "types.h"
#include "utils.h"


const struct SimulatedAnnealing_Hyperparameters_T simulated_annealing_default_hyperparameters = {
	1000000, // U32 first_cycle_len;
	10, // U32 recheck_rate;
	10000.0, // F64 score_diff_multiplier;
	1.5 // F64 cycle_multiplier
};

void simulated_annealing(struct Problem_T * problem, const struct SimulatedAnnealing_Hyperparameters_T * hyperparams, struct REPORTER_MEM_T * reporter_mem, double score_limit, U32 trial_limit, U8 * data_out, double * score_out, U32 * iterations_out)
{
  if (problem->trial_initializer)
  {
    problem->trial_initializer(problem);
  }
  
  
	U8 * data = malloc(problem->data_len);
	U8 * data_b = malloc(problem->data_len);
	problem->data_initializer(problem, data);
	memcpy(data_b, data, problem->data_len);

	float score = problem->scalar_trial(problem, data);
	float current_score_total = score;
	int current_score_count = 1;

	int last_batch_score = 0;
	U64 last_batch_iterations = 0;
	int batch_num = 0;

	U64 max_iterations = hyperparams->first_cycle_len;

	if (reporter_mem)
	{
		sem_wait(&(reporter_mem->data_sem));
		memcpy(reporter_mem->current_data, data, problem->data_len);
		reporter_mem->enable_cycles = 1;
		reporter_mem->cycle_num = batch_num;
		reporter_mem->trials_in_cycle = max_iterations;
		reporter_mem->trials_completed_in_cycle = 0;
		sem_post(&(reporter_mem->data_sem));
	}

	U64 total_tests = 0;
	while (1) {
	    long i;
	    for (i = 0; i < max_iterations; i++)
	    {
	    	//sanity_check(data);
	    	// Copy data to data_b and modify it
	    	memcpy(data_b, data, problem->data_len);

	    	if ((i%hyperparams->recheck_rate) == 0)
	    	{

				// Average prev score with new run
				current_score_total += problem->scalar_trial(problem, data);
				current_score_count += 1;
				score = current_score_total/current_score_count;

				total_tests++;
	    	}

			/* Make a random change and introduce a new agent */
	    	problem->scrambler(problem, data);

	    	// Score run
	    	float new_score = problem->scalar_trial(problem, data);
	    	total_tests++;

	    	// Simulated annealing to determine if this passes
	    	int does_pass = 0;
	    	if (new_score >= score)
	    	{
	    		does_pass = 1;
	    	}
	    	else {
	    		float t = (float)max_iterations/(float)(i);
	    		float p = exp((float)((new_score - score)*hyperparams->score_diff_multiplier)/(t));
	        	does_pass = fast_rand() < (32767.0 * p);
	        }
		    if (does_pass) {
				// Keep the new code
				score = new_score;
				current_score_total = score;
				current_score_count = 1;
	        }
		    else
		    {
				// Revert to the old code
				U8* t = data_b;
				data_b = data;
				data = t;
	        }
	    	if (reporter_mem)
	    	{
	    		if (!(i % 10)) {
	    			sem_wait(&(reporter_mem->data_sem));
	    			memcpy(reporter_mem->current_data, data, problem->data_len);
	    			reporter_mem->enable_cycles = 1;
	    			reporter_mem->cycle_num = batch_num;
	    			reporter_mem->trials_in_cycle = max_iterations;
	    			reporter_mem->trials_completed_in_cycle = i;
	    			reporter_mem->trials_completed = total_tests;
	    			sem_post(&(reporter_mem->data_sem));
	    		}
		    	if (reporter_mem->abort_solve)
		    	{
		    		break;
		    	}
	    	}

		    if (score >= score_limit)
		    {
		    	break;
		    }
		    if (total_tests >= trial_limit)
		    {
		    	break;
		    }

	    }

    	if (reporter_mem)
    	{
    		sem_wait(&(reporter_mem->data_sem));
    		memcpy(reporter_mem->current_data, data, problem->data_len);
    		reporter_mem->enable_cycles = 1;
    		reporter_mem->cycle_num = batch_num;
    		reporter_mem->trials_in_cycle = max_iterations;
    		reporter_mem->trials_completed_in_cycle = i;
    		reporter_mem->trials_completed = total_tests;
    		sem_post(&(reporter_mem->data_sem));
	    	if (reporter_mem->abort_solve)
	    	{
	    		break;
	    	}
    	}

    	if (score >= score_limit)
    	{
    		break;
    	}
    	if (total_tests >= trial_limit)
    	{
    		break;
    	}
	    last_batch_score = score;
	    last_batch_iterations = max_iterations;
	    max_iterations = max_iterations * hyperparams->cycle_multiplier;
	    batch_num++;
	  }


  if (problem->trial_deinitializer)
  {
    problem->trial_deinitializer(problem);
  }

  if (score_out)
  {
    *score_out = score;
  }
  if (iterations_out)
  {
    *iterations_out = total_tests;
  }
  if (data_out)
  {
    memcpy(data_out, data, problem->data_len);
  }
  free(data);
  free(data_b);
}
