#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "solvers/solvers.h"
#include "problems/problems.h"
#include "reporters/reporter.h"
#include "types.h"
#include "utils.h"

void simple_greedy(struct Problem_T * problem, struct REPORTER_MEM_T * reporter_mem, double score_limit, U32 trial_limit, U8 * data_out, double * score_out, U32 * iterations_out)
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

	if (reporter_mem)
	{
		sem_wait(&(reporter_mem->data_sem));
		memcpy(reporter_mem->current_data, data, problem->data_len);
		sem_post(&(reporter_mem->data_sem));
	}

	U64 total_tests = 0;
	while (1)
	{
	    // Copy data to data_b and modify it
	    memcpy(data_b, data, problem->data_len);

		/* Make a random change and introduce a new agent */
	    problem->scrambler(problem, data);

	    // Score run
	    float new_score = problem->scalar_trial(problem, data);
	    total_tests++;

	   	// Simulated annealing to determine if this passes
	    int does_pass = 0;
	    if (new_score >= score)
	    {
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
	    	if (!(total_tests % 10))
	    	{
	    		sem_wait(&(reporter_mem->data_sem));
	    		memcpy(reporter_mem->current_data, data, problem->data_len);
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
