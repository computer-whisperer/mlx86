#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "solvers/solvers.h"
#include "problems/problems.h"
#include "types.h"
#include "utils.h"


const struct SimpleGenetic_Hyperparameters_T simple_genetic_hyperparameters = {

};



void simple_genetic(struct Problem_T * problem, const struct SimpleGenetic_Hyperparameters_T * hyperparams, U8 reporting, double score_limit, U32 trial_limit, U8 * data_out, double * score_out, U32 * iterations_out)
{
	  if (problem->trial_initializer)
	  {
		problem->trial_initializer(problem);
	  }


	U8 * data = malloc(problem->data_len);
	U8 * data_b = malloc(problem->data_len);
	problem->data_initializer(problem, data);
	memcpy(data_b, data, problem->data_len);

	float score = problem->scalar_trial(problem, data);

	double last_update = getUnixTime();


	U64 max_iterations = hyperparams->first_cycle_len;
	U64 total_tests = 0;



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
}
