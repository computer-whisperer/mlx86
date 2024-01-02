#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "problems/problem.h"
#include "testing.h"
#include "types.h"
#include "christian_utils.h"
#include "ProblemHelloWorld.h"

double ProblemHelloWorld::scalarTrial(U8 * data)
{
  float score = 0;

  U32 l = data_len;
  if (l > strlen(goal))
  {
	  l = strlen(goal);
  }

  for (int i = 0; i < l; i++)
  {
    int error = goal[i] - data[i];
    if (error < 0)
      error = -error;
    if (error > 20)
      error = 20;
    if (error == 0) {
      score += 2;
    }
    score += 1.0f-(static_cast<float>(error)/20.0f);
  }

  float out = (score/3)/((double)l);

	// Determinism test
    //TESTING_HASH_T input_hash = TESTING_STARTING_HASH;
	//TESTING_ADD_DATA_TO_HASH(input_hash, data, data_len);
	//TESTING_HASH_T output_hash = TESTING_STARTING_HASH;
	//TESTING_ADD_VALUE_TO_HASH(output_hash, out);
	//testing_assert_determinism("scalar_trial_hello_world_determinism", input_hash, output_hash);
  return out;
}

void ProblemHelloWorld::prettyPrintData(U8 *data) {
    Problem::textPrettyPrint(data);
}

void ProblemHelloWorld::dataInit(U8 *data) {
    Problem::textDataInit(data);
}

void ProblemHelloWorldNonzero::prettyPrintData(U8 *data) {
    Problem::textPrettyPrint(data);
}

double ProblemHelloWorldNonzero::scalarTrial(U8 * data)
{
    float score = 0;
    for (int i = 0; i < data_len; i++)
    {
        if (data[i])
        {
            score += 1;
        }
    }


    float out = score/data_len;
    // Determinism test
    /*
	TESTING_HASH_T input_hash = TESTING_STARTING_HASH;
	TESTING_ADD_DATA_TO_HASH(input_hash, problem, sizeof(struct Problem_T));
	TESTING_ADD_DATA_TO_HASH(input_hash, data, problem->data_len);
	TESTING_HASH_T output_hash = TESTING_STARTING_HASH;
	TESTING_ADD_VALUE_TO_HASH(output_hash, out);
	testing_assert_determinism("scalar_trial_nonzero_determinism", input_hash, output_hash)
	 */
    return out;
}

void ProblemHelloWorldNonzero::dataInit(U8 *data) {
    Problem::textDataInit(data);
}

