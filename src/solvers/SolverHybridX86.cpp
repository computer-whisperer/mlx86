#include <cstring>
#include <cstdlib>
#include <cstddef>
#include <X86Common.h>
#include <ProblemTravellingSalesman.h>
#include "solvers/Solver.h"
#include "problems/problem.h"
#include "reporters/reporter.h"
#include "KVMExecutor.h"
#include "types.h"
#include "SolverHybridX86.h"

#define MAX_SUB_PROBLEM_DATA 0x1000
#define SCRATCH_MEM_LEN 0x1000


SolverHybridX86::SolverHybridX86(size_t code_len_in):
Problem((code_len_in < max_code_len)?code_len_in:max_code_len),
code(nullptr),
executor(nullptr)
{}

SolverHybridX86::SolverHybridX86(const SolverHybridX86 &other)
: Problem(other.data_len), executor(nullptr), code(nullptr) {}

SolverHybridX86::~SolverHybridX86() {
    delete executor;
}

SolverHybridX86 * SolverHybridX86::Clone()
{
    return new SolverHybridX86(*this);
}

void SolverHybridX86::run(Problem *problem, struct REPORTER_MEM_T * reporter_mem, double score_limit, U32 trial_limit, struct SolverResults_T * results_out)
{

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
		U8 data[MAX_SUB_PROBLEM_DATA];
		U8 scratch_mem[SCRATCH_MEM_LEN];
	};

	if (executor == nullptr)
	{
	    executor = new KVMExecutor(sizeof(struct io_memory_map_t), data_len);
	}

	// Copy program into vm memory
	U8 * program_memory = executor->program_memory;
	memcpy(program_memory, code, data_len);

	// Setup initial IO memory
	auto * io_memory = (struct io_memory_map_t *)executor->io_memory;
	problem->dataInit(io_memory->data);

	U8 * prev_data = static_cast<U8 *>(malloc(problem->data_len));
	U8 * prev_prev_data = static_cast<U8 *>(malloc(problem->data_len));

	if (reporter_mem)
	{
	    reporter_mem->mtx.lock();
		memcpy(reporter_mem->current_data, io_memory->data, problem->data_len);
		reporter_mem->mtx.unlock();
	}

	U64 total_tests = 0;
	F64 current_score;
	while (true)
	{
		// Grab copy for possible future revert
		U8 * c = prev_prev_data;
		prev_prev_data = prev_data;
		prev_data = c;
		memcpy(prev_data, io_memory->data, problem->data_len);

		// Get current score
		current_score = problem->scalarTrial(io_memory->data);
		total_tests++;

		// Setup io memory and run program
		io_memory->do_revert_offset = offsetof(struct io_memory_map_t, do_revert);
		io_memory->do_scramble_offset = offsetof(struct io_memory_map_t, do_revert);
		io_memory->score_offset = offsetof(struct io_memory_map_t, score);
		io_memory->data_offset = offsetof(struct io_memory_map_t, data);
		io_memory->scratch_mem_offset = offsetof(struct io_memory_map_t, scratch_mem);
		io_memory->score = current_score;

		// Run program
		int did_hang = executor->run();

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
			problem->scrambler(io_memory->data);
		}

	    if (reporter_mem)
	    {
	    	if (!(total_tests % 10))
	    	{
	    		reporter_mem->mtx.lock();
	    		memcpy(reporter_mem->current_data, io_memory->data, problem->data_len);
	    		reporter_mem->trials_completed = total_tests;
	    		reporter_mem->mtx.unlock();
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

	if (results_out)
	{
		results_out->data = static_cast<U8 *>(malloc(problem->data_len));
		memcpy(results_out->data, io_memory->data, problem->data_len);
		results_out->score = current_score;
		results_out->trial_count = total_tests;
	}

	free(prev_data);
	free(prev_prev_data);
}

double SolverHybridX86::scalarTrial(U8 *data) {
    code = data;
    return ProblemTravellingSalesman::SolverTest(this);
}

void SolverHybridX86::scrambler(U8 *data) {
    x86_basic_scramble(data, data_len);
}

void SolverHybridX86::prettyPrintData(U8 *data) {
    x86_pretty_print(data, data_len);
}

void SolverHybridX86::dataInit(U8 *data) {
    x86_data_init(data, data_len);
}
