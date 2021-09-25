#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "problems/problem.h"
#include "solvers/Solver.h"
#include "utils.h"
#include "types.h"

static float scalar_trial(struct Problem_T * problem, U8 * data)
{

	struct SolverResults_T results;
  // Preserve and reset rng state so everything stays deterministic
  U64 prev_seed = get_fast_rand_seed();
  fast_rand_seeded(4819);
  simulated_annealing(problem->meta_problem, (struct SimulatedAnnealing_Hyperparameters_T *) data, NULL, 1, 10000, &results);
  // Restore previous rng state
  fast_rand_seeded(prev_seed);
  free(results.data);
  return (double)results.score - (double)results.trial_count/1000.0;
}

static void scramble(struct Problem_T * problem, U8 * data)
{
	struct SimulatedAnnealing_Hyperparameters_T * hyperparams = (struct SimulatedAnnealing_Hyperparameters_T *)data;
	U8 kind = fast_rand()%4;
	double positive_multiplier = (((double)fast_rand())/32767.0) + 1;
	if (fast_rand()%2)
	{
		// Up
		positive_multiplier = 1/positive_multiplier;
	}


	switch (kind)
	{
	case 0:
		hyperparams->first_cycle_len = ((double)hyperparams->first_cycle_len)*positive_multiplier;
		break;
	case 1:
		hyperparams->recheck_rate = ((double)hyperparams->recheck_rate)*positive_multiplier;
		break;
	case 2:
		hyperparams->score_diff_multiplier *= positive_multiplier;
		break;
	case 3:
		hyperparams->cycle_multiplier *= positive_multiplier;
		break;
	}

	// Limiting
	if (hyperparams->recheck_rate < 4)
	{
		hyperparams->recheck_rate = 4;
	}
	if (hyperparams->first_cycle_len < 10)
	{
		hyperparams->first_cycle_len = 10;
	}
  if (hyperparams->score_diff_multiplier < 1)
	{
		hyperparams->score_diff_multiplier = 1;
	}
	if (hyperparams->cycle_multiplier < 1)
	{
		hyperparams->cycle_multiplier = 1;
	}
}

static void pretty_print(struct Problem_T * problem, U8 * data)
{
	struct SimulatedAnnealing_Hyperparameters_T * hyperparams = (struct SimulatedAnnealing_Hyperparameters_T *)data;
	printf("{\n");
	printf("%d   //U32 first_cycle_len;\n", hyperparams->first_cycle_len);
	printf("%d   //U32 recheck_rate;\n", hyperparams->recheck_rate);
	printf("%f   //F64 score_diff_multiplier;\n", hyperparams->score_diff_multiplier);
	printf("%f   //F64 cycle_multiplier;\n", hyperparams->cycle_multiplier);
	printf("}\n");
}

static void data_init(struct Problem_T * problem, U8 * data)
{
	memcpy(data, &simulated_annealing_default_hyperparameters, problem->data_len);
}

struct Problem_T problem_simulated_annealing_hyperparameters =
{
  NULL,
  NULL,
	scalar_trial,
	scramble,
	pretty_print,
	data_init,
	sizeof(struct SimulatedAnnealing_Hyperparameters_T),
	&problem_hello_world
};

