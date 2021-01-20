#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "problems/problems.h"
#include "problems/common.h"
#include "testing.h"
#include "types.h"
#include "utils.h"

float scalar_trial_hello_world(struct Problem_T * problem, U8 * data)
{
  float score = 0;

  const char * goal = "I am mlx86!!!12345678910abcdefghijklmnopqrstuvwxyz";

  U32 l = problem->data_len;
  if (l > strlen(goal))
  {
	  l = strlen(goal);
  }

  for (int i = 0; i < l; i++)
  {
    int error = goal[i] - data[i];
    if (error < 0)
      error = -error;
    if (error > 10)
      error = 10;
    score += 1.0-(((float)error)/10.0);
  }

  float out = score/((double)l);

	// Determinism test
    TESTING_HASH_T input_hash = TESTING_STARTING_HASH;
	TESTING_ADD_DATA_TO_HASH(input_hash, problem, sizeof(struct Problem_T));
	TESTING_ADD_DATA_TO_HASH(input_hash, data, problem->data_len);
	TESTING_HASH_T output_hash = TESTING_STARTING_HASH;
	TESTING_ADD_VALUE_TO_HASH(output_hash, out);
	testing_assert_determinism("scalar_trial_hello_world_determinism", input_hash, output_hash);

  return out;
}

float scalar_trial_nonzero(struct Problem_T * problem, U8 * data)
{
  float score = 0;
  for (int i = 0; i < problem->data_len; i++)
  {
		if (data[i])
		{
			score += 1;
		}
	}


	float out = score/problem->data_len;

	// Determinism test
	TESTING_HASH_T input_hash = TESTING_STARTING_HASH;
	TESTING_ADD_DATA_TO_HASH(input_hash, problem, sizeof(struct Problem_T));
	TESTING_ADD_DATA_TO_HASH(input_hash, data, problem->data_len);
	TESTING_HASH_T output_hash = TESTING_STARTING_HASH;
	TESTING_ADD_VALUE_TO_HASH(output_hash, out);
	testing_assert_determinism("scalar_trial_nonzero_determinism", input_hash, output_hash);

	return out;
}

struct Problem_T problem_nonzero =
{
  NULL,
  NULL,
	scalar_trial_nonzero,
	pure_random_scramble,
	text_pretty_print,
	text_data_init,
	20,
	0
};

struct Problem_T problem_hello_world =
{
  NULL,
  NULL,
	scalar_trial_hello_world,
	pure_random_scramble,
	text_pretty_print,
	text_data_init,
	13,
	0
};


