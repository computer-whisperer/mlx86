/*
 * simulated_annealing.c
 *
 *  Created on: Nov 16, 2020
 *      Author: christian
 */

#include <cstring>
#include <cmath>
#include "solvers/Solver.h"
#include "problems/problem.h"
#include "types.h"
#include "christian_utils.h"
#include "SolverSimulatedAnnealing.h"

void SolverSimulatedAnnealing::run(Problem *problem, struct REPORTER_MEM_T * reporter_mem, double score_limit, U32 trial_limit, struct SolverResults_T * results_out)
{
	U8 * data = static_cast<U8 *>(malloc(problem->data_len));
	U8 * data_b = static_cast<U8 *>(malloc(problem->data_len));
	problem->dataInit(data);
	memcpy(data_b, data, problem->data_len);

	double score = problem->scalarTrial(data);
	double current_score_total = score;
	int current_score_count = 1;

	int batch_num = 0;

	U64 max_iterations = first_cycle_len;

	if (reporter_mem)
	{
	    reporter_mem->mtx.lock();
		memcpy(reporter_mem->current_data, data, problem->data_len);
		reporter_mem->enable_cycles = 1;
		reporter_mem->cycle_num = batch_num;
		reporter_mem->trials_in_cycle = max_iterations;
		reporter_mem->trials_completed_in_cycle = 0;
		reporter_mem->mtx.unlock();
	}

	U64 total_tests = 0;
	while (true) {
		if (max_iterations > (trial_limit - total_tests))
		{
			max_iterations = (trial_limit - total_tests);
		}

	    long i;
	    for (i = 0; i < max_iterations; i++)
	    {
	    	//sanity_check(data);
	    	// Copy data to data_b and modify it
	    	memcpy(data_b, data, problem->data_len);

	    	if ((i%recheck_rate) == 0)
	    	{

				// Average prev score with new run
				current_score_total += problem->scalarTrial(data);
				current_score_count += 1;
				score = current_score_total/current_score_count;

				total_tests++;
	    	}

			/* Make a random change and introduce a new agent */
	    	problem->scrambler(data);

	    	// Score run
	    	double new_score = problem->scalarTrial(data);
	    	total_tests++;

	    	// Simulated annealing to determine if this passes
	    	int does_pass;
	    	if (new_score >= score)
	    	{
	    		does_pass = 1;
	    	}
	    	else {
	    	    double t = (double)max_iterations/(double)(i);
	    		double p = exp(((new_score - score)*score_diff_multiplier)/(t));
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
	    		    reporter_mem->mtx.lock();
	    			memcpy(reporter_mem->current_data, data, problem->data_len);
	    			reporter_mem->enable_cycles = 1;
	    			reporter_mem->cycle_num = batch_num;
	    			reporter_mem->trials_in_cycle = max_iterations;
	    			reporter_mem->trials_completed_in_cycle = i;
	    			reporter_mem->trials_completed = total_tests;
	    			reporter_mem->mtx.unlock();
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
    	    reporter_mem->mtx.lock();
    		memcpy(reporter_mem->current_data, data, problem->data_len);
    		reporter_mem->enable_cycles = 1;
    		reporter_mem->cycle_num = batch_num;
    		reporter_mem->trials_in_cycle = max_iterations;
    		reporter_mem->trials_completed_in_cycle = i;
    		reporter_mem->trials_completed = total_tests;
    		reporter_mem->mtx.unlock();
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
	    max_iterations = (U64)((double)max_iterations * cycle_multiplier);
	    batch_num++;
	  }


  	if (results_out)
	{
		results_out->data = static_cast<U8 *>(malloc(problem->data_len));
		memcpy(results_out->data, data, problem->data_len);
		results_out->score = score;
		results_out->trial_count = total_tests;
	}
	free(data);
	free(data_b);
}