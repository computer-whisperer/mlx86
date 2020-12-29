#include <stdio.h>
#include "problems/problems.h"
#include "problems/common.h"
#include "types.h"

float scalar_trial_hello_world(struct Problems_T * problem, U8 * data)
{
  float score = 0;

  char * goal = "I am mlx86!!!";

  for (int i = 0; i < strlen(goal); i++) {
    int error = goal[i] - data[i];
    if (error < 0)
      error = -error;
    if (error > 10)
      error = 10;
    score += 1.0-(((float)error)/10.0);
  }

  return score/strlen(goal);
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

	return score/problem->data_len;
}

struct Problem_T problem_nonzero =
{
	scalar_trial_nonzero,
	pure_random_scramble,
	text_pretty_print,
	text_data_init,
	20,
	0
};

struct Problem_T problem_hello_world =
{
	scalar_trial_hello_world,
	pure_random_scramble,
	text_pretty_print,
	text_data_init,
	20,
	0
};


