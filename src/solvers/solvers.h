#ifndef SOLVERS_H
#define SOLVERS_H
#include "reporters/reporter.h"
#include "problems/problems.h"
#include "types.h"


struct SolverResults_T
{
	F64 score;
	U64 trial_count;
	U8 * data;
};

struct Solver_T
{
	void (*solver)(struct Solver_T * solver, struct Problem_T * problem, struct REPORTER_MEM_T * reporter_mem, double score_limit, U32 trial_limit, struct SolverResults_T * results_out);
	size_t hyperparameters_size;
	void * hyperparameters;
};

extern struct Solver_T solver_simulated_annealing;
extern struct Solver_T solver_parallel_tempering;
extern struct Solver_T solver_parallel_tempering_slow;
extern struct Solver_T solver_simple_greedy;
extern struct Solver_T solver_tabu_search;
struct Solver_T * build_solver_hybrid_x86(void * data, size_t data_len);


#endif
