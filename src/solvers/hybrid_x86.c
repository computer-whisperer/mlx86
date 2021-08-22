#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include "solvers/solvers.h"
#include "problems/problems.h"
#include "reporters/reporter.h"
#include "executors/executor.h"
#include "types.h"
#include "utils.h"

#define SCRATCH_MEM_LEN 0x1000

void hybrid_x86(struct Solver_T * solver, struct Problem_T * problem, struct REPORTER_MEM_T * reporter_mem, double score_limit, U32 trial_limit, struct SolverResults_T * results_out)
{
	if (problem->trial_initializer)
	{
		problem->trial_initializer(problem);
	}

	struct io_memory_map_t
	{
		U32 do_revert_offset;
		U32 do_scramble_offset;
		U32 score_offset;
		U32 data_offset;
		U32 scratch_mem_offset;
		U32 do_revert;
		U32 do_scramble;
		F64 score;
		U8 data[problem->data_len];
		U8 scratch_mem[SCRATCH_MEM_LEN];
	};

	static struct EXECUTOR_DATA_T executor_data;
	executor_init(&executor_data, sizeof(struct io_memory_map_t), solver->hyperparameters_size);

	// Copy program into vm memory
	U8 * program_memory = EXECUTOR_PROGRAM_MEM(&executor_data);
	memcpy(program_memory, solver->hyperparameters, solver->hyperparameters_size);

	// Setup initial IO memory
	struct io_memory_map_t * io_memory = EXECUTOR_IO_MEM(&executor_data);
	problem->data_initializer(problem, io_memory->data);

	U8 * prev_data = malloc(problem->data_len);
	U8 * prev_prev_data = malloc(problem->data_len);

	if (reporter_mem)
	{
		sem_wait(&(reporter_mem->data_sem));
		memcpy(reporter_mem->current_data, io_memory->data, problem->data_len);
		sem_post(&(reporter_mem->data_sem));
	}

	U64 total_tests = 0;
	F64 current_score = 0;
	while (1)
	{
		// Grab copy for possible future revert
		U8 * c = prev_prev_data;
		prev_prev_data = prev_data;
		prev_data = c;
		memcpy(prev_data, io_memory->data, problem->data_len);

		// Get current score
		current_score = problem->scalar_trial(problem, io_memory->data);
		total_tests++;

		// Setup io memory and run program
		io_memory->do_revert_offset = offsetof(struct io_memory_map_t, do_revert);
		io_memory->do_scramble_offset = offsetof(struct io_memory_map_t, do_revert);
		io_memory->score_offset = offsetof(struct io_memory_map_t, score);
		io_memory->data_offset = offsetof(struct io_memory_map_t, data);
		io_memory->scratch_mem_offset = offsetof(struct io_memory_map_t, scratch_mem);
		io_memory->score = current_score;

		// Run program
		int did_hang = executor_execute(&executor_data);

		if (did_hang)
		{
			// Abort!
			break;
		}

		if (io_memory->do_revert)
		{
			// Revert to prev prev data if requested
			memcpy(io_memory->data, prev_prev_data, problem->data_len);
		}

		if (io_memory->do_scramble)
		{
			// Revert to prev prev data if requested
			problem->scrambler(problem, io_memory->data);
		}

	    if (reporter_mem)
	    {
	    	if (!(total_tests % 10))
	    	{
	    		sem_wait(&(reporter_mem->data_sem));
	    		memcpy(reporter_mem->current_data, io_memory->data, problem->data_len);
	    		reporter_mem->trials_completed = total_tests;
	    		sem_post(&(reporter_mem->data_sem));
	    	}
	    	if (reporter_mem->abort_solve)
	    	{
	    		break;
	    	}
	    }

		if (current_score >= score_limit)
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

	if (results_out)
	{
		results_out->data = malloc(problem->data_len);
		memcpy(results_out->data, io_memory->data, problem->data_len);
		results_out->score = current_score;
		results_out->trial_count = total_tests;
	}

	executor_deinit(&executor_data);

	free(prev_data);
	free(prev_prev_data);
}

struct Solver_T * build_solver_hybrid_x86(void * data, size_t data_len)
{
	struct Solver_T * solver_hybrid_x86 = malloc(sizeof(struct Solver_T));
	solver_hybrid_x86->solver = hybrid_x86;
	solver_hybrid_x86->hyperparameters_size = data_len;
	solver_hybrid_x86->hyperparameters = data;
	return solver_hybrid_x86;
}


