#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "problems/problems.h"
#include "solvers/solvers.h"
#include "utils.h"
#include "types.h"

static float scalar_trial(struct Problem_T * problem, U8 * data)
{

  double score;
  U32 iterations;
  parallel_tempering(problem->meta_problem, (struct ParallelTempering_Hyperparameters_T *) data, 0, 1, 10000, NULL, &score, &iterations);
  return (double)score - (double)iterations/1000.0;
}

static void scramble(struct Problem_T * problem, U8 * data)
{
	struct ParallelTempering_Hyperparameters_T * hyperparams = (struct ParallelTempering_Hyperparameters_T *)data;
	U8 kind = fast_rand()%8;
	double positive_multiplier = (((double)fast_rand())/32767.0) + 1;
	if (fast_rand()%2)
	{
		// Up
		positive_multiplier = 1/positive_multiplier;
	}

	switch (kind)
	{
	case 0:
		hyperparams->cycle_len = ((double)hyperparams->cycle_len)*positive_multiplier;
		break;
	case 1:
		hyperparams->recheck_rate = ((double)hyperparams->recheck_rate)*positive_multiplier;
		break;
	case 2:
		hyperparams->score_diff_multiplier *= positive_multiplier;
		break;
	case 3:
		hyperparams->neighbor_post_rate = ((double)hyperparams->neighbor_post_rate)*positive_multiplier;
		break;
	case 4:
		hyperparams->neighbor_poll_rate = ((double)hyperparams->neighbor_poll_rate)*positive_multiplier;
		break;
	case 5:
		hyperparams->neighbor_poll_chance = ((double)hyperparams->neighbor_poll_chance)*positive_multiplier;
		break;
	case 6:
		hyperparams->score_diff_neighbor_multiplier = ((double)hyperparams->score_diff_neighbor_multiplier)*positive_multiplier;
		break;
	case 7:
		hyperparams->num_neighbors = ((double)hyperparams->num_neighbors)*positive_multiplier;
		break;

	}

	// Limiting
	if (hyperparams->recheck_rate < 4)
	{
		hyperparams->recheck_rate = 4;
	}
	if (hyperparams->cycle_len < 10)
	{
		hyperparams->cycle_len = 10;
	}
	if (hyperparams->score_diff_multiplier < 1)
	{
		hyperparams->score_diff_multiplier = 1;
	}
	if (hyperparams->neighbor_post_rate < 1)
	{
		hyperparams->neighbor_post_rate = 1;
	}
	if (hyperparams->neighbor_poll_chance < 1)
	{
		hyperparams->neighbor_poll_chance = 1;
	}
	if (hyperparams->score_diff_neighbor_multiplier < 1)
	{
		hyperparams->score_diff_neighbor_multiplier = 1;
	}
	if (hyperparams->num_neighbors < 2)
	{
		hyperparams->num_neighbors = 2;
	}
	if (hyperparams->num_neighbors > 5)
	{
		hyperparams->num_neighbors = 5;
	}
}

static void pretty_print(struct Problem_T * problem, U8 * data)
{
	struct ParallelTempering_Hyperparameters_T * hyperparams = (struct ParallelTempering_Hyperparameters_T *)data;
	printf("{\n");
	printf("%d   //U32 cycle_len;\n", hyperparams->cycle_len);
	printf("%d   //U32 recheck_rate;\n", hyperparams->recheck_rate);
	printf("%f   //F64 score_diff_multiplier;\n", hyperparams->score_diff_multiplier);
  printf("%d   //U32 neighbor_post_rate;\n", hyperparams->neighbor_post_rate);
  printf("%d   //U32 neighbor_poll_rate;\n", hyperparams->neighbor_poll_rate);
  printf("%d   //U32 neighbor_poll_chance;\n", hyperparams->neighbor_poll_chance);
  printf("%f   //F64 score_diff_neighbor_multiplier;\n", hyperparams->score_diff_neighbor_multiplier);
  printf("%d   //U32 num_neighbors;\n", hyperparams->num_neighbors);
	printf("}\n");
}

static void data_init(struct Problem_T * problem, U8 * data)
{
	memcpy(data, &parallel_tempering_default_hyperparameters, problem->data_len);
}

struct Problem_T problem_parallel_tempering_hyperparameters =
{
  NULL,
  NULL,
	scalar_trial,
	scramble,
	pretty_print,
	data_init,
	sizeof(struct ParallelTempering_Hyperparameters_T),
	&problem_hello_world
};

