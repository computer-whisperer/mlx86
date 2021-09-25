#include <string.h>
#include <stdlib.h>
#include "problems/problem.h"
#include "solvers/Solver.h"
#include "problems/x86/X86Common.h"
#include "utils.h"
#include "types.h"

#define NUM_PROBLEMS_PER_TEST 2
#define NUM_NODES 20

static float scalar_trial(struct Problem_T * problem, U8 * data)
{
	struct SolverResults_T results;
	struct Solver_T * solver = build_solver_hybrid_x86(data, problem->data_len);
	F64 score = 0;
	for (U32 i = 0; i < NUM_PROBLEMS_PER_TEST; i++)
	{
		solver->solver(solver, ((struct Problem_T **)problem->supporting_data)[i], NULL, 1, 100, &results);
		free(results.data);
		score += results.score/(F64)NUM_PROBLEMS_PER_TEST;
	}
	return score;
}

struct Problem_T * new_problem_hybrid_x86_hyperparameters()
{
	struct Problem_T * problem = static_cast<Problem_T *>(malloc(sizeof(struct Problem_T)));
	problem->trial_initializer = NULL;
	problem->trial_deinitializer = NULL;
	problem->scalar_trial = scalar_trial;
	problem->scrambler = x86_basic_scramble;
	problem->data_pretty_printer = x86_pretty_print;
	problem->data_initializer = x86_data_init;
	problem->data_len = 0x800;
	problem->supporting_data = malloc(sizeof(struct Problem_T *)*NUM_PROBLEMS_PER_TEST);
	for (U32 i = 0; i < NUM_PROBLEMS_PER_TEST; i++)
	{
		((struct Problem_T **)problem->supporting_data)[i] = travelling_salesman_new_problem(NUM_NODES, i);
	}
	return problem;
}

