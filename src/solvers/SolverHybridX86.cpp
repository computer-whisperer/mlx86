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

#include <christian_utils.h>
#include <cmath>
#include <ProblemHelloWorld.h>

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

void SolverHybridX86::run(Problem *problem, struct REPORTER_MEM_T * reporter_mem, double score_limit, U32 trial_limit, struct SolverResults_T * results_out, uint8_t* starting_data)
{
	inner_rng_seed = 123;
	struct io_memory_map_t
	{
		uint32_t data_length;
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
	memset(io_memory, 0, sizeof(struct io_memory_map_t));
	problem->dataInit(io_memory->data);

	U8 * prev_data = static_cast<U8 *>(malloc(problem->data_len));
	memcpy(prev_data, io_memory->data, problem->data_len);

	if (reporter_mem)
	{
	    reporter_mem->mtx.lock();
		memcpy(reporter_mem->current_data, io_memory->data, problem->data_len);
		reporter_mem->mtx.unlock();
	}

	U64 total_tests = 0;
	F64 current_score = -1000;
	F64 prev_score = 0;
	while (true)
	{

		if (false && inner_rng()%30 == 0) {
			// Use random value
			problem->scrambler(io_memory->data);
		}
		else {
			// Use x86 kernel for mutation

			// Setup io memory and run program
			io_memory->data_length = problem->data_len;

			// Run program
			int did_hang = executor->run();

			if (did_hang)
			{
				current_score = -100;
				// Abort!
				break;
			}
		}

		// Get new score
		current_score = problem->scalarTrial(io_memory->data);

		// Get current score
		total_tests++;

		// Simulated annealing to determine if this passes
		int does_pass;
		if (current_score >= prev_score)
		{
			does_pass = 1;
		}
		else {
			double t = (double)trial_limit/(double)(total_tests);
			double p = exp(((current_score - prev_score)*10000.0)/(t));
			does_pass = fast_rand() < (32767.0 * p);
		}
		if (!does_pass) {
			// Revert to old values
			memcpy(io_memory->data, prev_data, problem->data_len);
		}
		else {
			// Save as prev values
			memcpy(prev_data, io_memory->data, problem->data_len);
			prev_score = current_score;
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
}

double SolverHybridX86::scalarTrial(U8 *data) {
    code = data;
		struct SolverResults_T results{};
		double score = 0;
		Problem * problems[] = {
			new ProblemHelloWorld("Hello there Obi-Wan Kenobi!!!"),
			new ProblemHelloWorld("I am mlx86!!!"),
			new ProblemHelloWorld("Hello World!!!")
		};

		for (int i = 0; i < std::size(problems); i++)
		{
			Problem* training_problem = problems[i];
			run(training_problem, nullptr, 100, 2000, &results);
			free(results.data);
			score += results.score;
		}
		return score/(double)std::size(problems);
}

void SolverHybridX86::scrambler(U8 *data) {
   // pureRandomScramble(data);
	 x86_basic_scramble(data, data_len);
}

void SolverHybridX86::prettyPrintData(U8 *data) {
    x86_pretty_print(data, data_len);
}

void SolverHybridX86::dataInit(U8 *data) {
    x86_data_init(data, data_len);
}

uint32_t SolverHybridX86::inner_rng() {
	inner_rng_seed = (214013*inner_rng_seed+2531011);
	return (inner_rng_seed>>16)&0x7FFF;
}
