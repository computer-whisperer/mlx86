#include <cstdlib>
#include <cstring>
#include "solvers/Solver.h"
#include "problems/problem.h"
#include "reporters/reporter.h"
#include "types.h"
#include "christian_utils.h"
#include "SolverTabuSearch.h"

#define TABU_LEN 10
#define NEIGHBOR_NUM 50

void SolverTabuSearch::run(Problem *problem, struct REPORTER_MEM_T * reporter_mem, double score_limit, U32 trial_limit, struct SolverResults_T * results_out, uint8_t* starting_data)
{

	U8 * data = static_cast<U8 *>(malloc(problem->data_len));
	U8 * data_b = static_cast<U8 *>(malloc(problem->data_len));
	U8 * data_c = static_cast<U8 *>(malloc(problem->data_len));
  if (starting_data) {
    memcpy(data, starting_data, problem->data_len);
  }
  else
  {
    problem->dataInit(data);
  }
	U32 data_hash = qhashmurmur3_32(data, problem->data_len);

	U32 tabu_list[TABU_LEN];
	U32 next_tabu_slot = 1;
	memset(tabu_list, 0, sizeof(tabu_list));
	tabu_list[0] = data_hash;

	double score;// = problem->scalarTrial(data);

	if (reporter_mem)
	{
	    reporter_mem->mtx.lock();
		memcpy(reporter_mem->current_data, data, problem->data_len);
		reporter_mem->mtx.unlock();
	}

	U64 total_tests = 1;
	while (true)
	{
	    // Copy data to data_c and modify it as first option
	    memcpy(data_c, data, problem->data_len);
	    problem->scrambler(data_c);
	    U32 hash_c = qhashmurmur3_32(data_c, problem->data_len);
	    double score_c = problem->scalarTrial(data_c);
	    total_tests++;

	    for (U32 i = 0; i < NEIGHBOR_NUM; i++)
	    {
	    	memcpy(data_b, data, problem->data_len);
		    problem->scrambler(data_b);
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
		    	F64 score_b = problem->scalarTrial(data_b);
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
    	score = score_c;
    	tabu_list[next_tabu_slot] = hash;
    	next_tabu_slot = (next_tabu_slot+1)%TABU_LEN;

	    if (reporter_mem)
	    {
	    	reporter_mem->mtx.lock();
	    	memcpy(reporter_mem->current_data, data, problem->data_len);
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
	free(data_c);
}