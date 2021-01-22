#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "solvers/solvers.h"
#include "problems/problems.h"
#include "reporters/reporter.h"
#include "types.h"
#include "utils.h"

#define TABU_LEN 100
#define NEIGHBOR_NUM 1000

void tabu_search(struct Problem_T * problem, struct REPORTER_MEM_T * reporter_mem, double score_limit, U32 trial_limit, U8 * data_out, double * score_out, U32 * iterations_out)
{
	if (problem->trial_initializer)
	{
		problem->trial_initializer(problem);
	}

	U8 * data = malloc(problem->data_len);
	U8 * data_b = malloc(problem->data_len);
	U8 * data_c = malloc(problem->data_len);
	problem->data_initializer(problem, data);
	U32 data_hash = qhashmurmur3_32(data, problem->data_len);

	U32 tabu_list[TABU_LEN];
	U32 next_tabu_slot = 1;
	memset(tabu_list, 0, sizeof(tabu_list));
	tabu_list[0] = data_hash;

	float score = problem->scalar_trial(problem, data);

	if (reporter_mem)
	{
		sem_wait(&(reporter_mem->data_sem));
		memcpy(reporter_mem->current_data, data, problem->data_len);
		sem_post(&(reporter_mem->data_sem));
	}

	U64 total_tests = 1;
	while (1)
	{
	    // Copy data to data_c and modify it as first option
	    memcpy(data_c, data, problem->data_len);
	    problem->scrambler(problem, data_c);
	    U32 hash_c = qhashmurmur3_32(data_c, problem->data_len);
	    F64 score_c = problem->scalar_trial(problem, data_c);
	    total_tests++;

	    for (U32 i = 0; i < NEIGHBOR_NUM; i++)
	    {
	    	memcpy(data_b, data, problem->data_len);
		    problem->scrambler(problem, data_b);
		    U32 hash_b = qhashmurmur3_32(data_b, problem->data_len);
		    U32 j;
		    for (j = 0; j < TABU_LEN; j++)
		    {
		    	if (tabu_list[j] == hash_b)
		    	{
		    		break;
		    	}
		    }
		    if (j == TABU_LEN)
		    {
		    	F64 score_b = problem->scalar_trial(problem, data_b);
		    	total_tests++;
		    	if (score_b > score_c)
		    	{
			    	memcpy(data_c, data_b, problem->data_len);
			    	hash_c = hash_b;
			    	score_c = score_b;
		    	}
		    }
	    }
	    // C is now the best option, mark it as tabu!
	    memcpy(data, data_c, problem->data_len);
    	U32 hash = hash_c;
    	U32 score = score_c;
    	tabu_list[next_tabu_slot] = hash;
    	next_tabu_slot = (next_tabu_slot+1)%TABU_LEN;

	    if (reporter_mem)
	    {
	    	sem_wait(&(reporter_mem->data_sem));
	    	memcpy(reporter_mem->current_data, data, problem->data_len);
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
	free(data_c);
}
