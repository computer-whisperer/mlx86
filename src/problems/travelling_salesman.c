#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "problems/problems.h"
#include "problems/common.h"
#include "utils.h"
#include "types.h"
#include "testing.h"

#define MAX_NODE_COUNT 1000
typedef U16 STATE_T;

struct SUPPORTING_DATA_T
{
	STATE_T node_count;
	U32 seed;
	double x_coords[MAX_NODE_COUNT];
	double y_coords[MAX_NODE_COUNT];
};

static float scalar_trial(struct Problem_T * problem, U8 * data)
{
	struct SUPPORTING_DATA_T * supporting_data = (struct SUPPORTING_DATA_T *)problem->supporting_data;

	STATE_T * steps = (STATE_T *)data;

	testing_assert("travelling_salesman_scalar_trial_1", supporting_data->node_count > 0);

	U8 state_track[supporting_data->node_count];
	memset(state_track, 0, supporting_data->node_count);
	U32 num_states_visited = 1;

	double len = 0;
	STATE_T last_state = steps[0];
	if (last_state > supporting_data->node_count)
	{
		last_state = 0;
	}
	for (STATE_T i = 1; i < supporting_data->node_count; i++)
	{
		STATE_T next_state = steps[i];
		if (next_state > supporting_data->node_count)
		{
			next_state = 0;
		}

		if (!state_track[next_state])
		{
			num_states_visited++;
		}

		state_track[next_state] = 1;

		double x_diff = supporting_data->x_coords[next_state] - supporting_data->x_coords[last_state];
		double y_diff = supporting_data->y_coords[next_state] - supporting_data->y_coords[last_state];
		double dist = sqrt(x_diff*x_diff + y_diff*y_diff);
		len += dist;
		last_state = next_state;
	}

	testing_assert("travelling_salesman_scalar_trial_2", len > 0);

	float cost = (len + (supporting_data->node_count - num_states_visited))/supporting_data->node_count;

	// Determinism test
	TESTING_HASH_T input_hash = TESTING_STARTING_HASH;
	TESTING_ADD_DATA_TO_HASH(input_hash, supporting_data, sizeof(struct SUPPORTING_DATA_T));
	TESTING_ADD_DATA_TO_HASH(input_hash, data, problem->data_len);
	TESTING_HASH_T output_hash = TESTING_STARTING_HASH;
	TESTING_ADD_DATA_TO_HASH(output_hash, supporting_data, sizeof(struct SUPPORTING_DATA_T));
	testing_assert_determinism("travelling_salesman_scalar_trial_determinism", input_hash, output_hash);


	return 1-cost;
}

static void scramble(struct Problem_T * problem, U8 * data)
{
	STATE_T * steps = (STATE_T *)data;
	struct SUPPORTING_DATA_T * supporting_data = (struct SUPPORTING_DATA_T *)problem->supporting_data;
	STATE_T a = fast_rand()%supporting_data->node_count;
	STATE_T b = fast_rand()%supporting_data->node_count;
	STATE_T c = steps[a];
	steps[a] = steps[b];
	steps[b] = c;
}

static void printer(struct Problem_T * problem, U8 * data)
{
	size_t len = problem->data_len;
	if (len > 100)
		len = 100;
	print_data_as_hex(data, len);
}

static void data_init(struct Problem_T * problem, U8 * data)
{
	STATE_T * steps = (STATE_T *)data;
	struct SUPPORTING_DATA_T * supporting_data = (struct SUPPORTING_DATA_T *)problem->supporting_data;

	STATE_T l = supporting_data->node_count;

	for (STATE_T i = 0; i < l; i++)
	{
		steps[i] = i;
	}
}

struct Problem_T * travelling_salesman_new_problem(U32 node_count, U32 seed)
{
	struct Problem_T * problem = malloc(sizeof(struct Problem_T));
	problem->trial_initializer = NULL;
	problem->trial_deinitializer = NULL;
	problem->scalar_trial = scalar_trial;
	problem->scrambler = scramble;
	problem->data_pretty_printer = printer;
	problem->data_initializer = data_init;
	problem->data_len = node_count*sizeof(STATE_T);
	struct SUPPORTING_DATA_T * supporting_data = malloc(sizeof(struct SUPPORTING_DATA_T));
	problem->supporting_data = supporting_data;

	// Swap out seed
	U32 prev_seed = get_fast_rand_seed();
	seed_fast_rand(seed);


	supporting_data->seed = seed;
	supporting_data->node_count = node_count;

	for (STATE_T i = 0; i < node_count; i++)
	{
		supporting_data->x_coords[i] = (double)fast_rand()/(double)FAST_RAND_MAX;
		supporting_data->y_coords[i] = (double)fast_rand()/(double)FAST_RAND_MAX;
	}


	// Restore old seed
	seed_fast_rand(prev_seed);

	// Determinism test
	TESTING_HASH_T input_hash = TESTING_STARTING_HASH;
	TESTING_ADD_VALUE_TO_HASH(input_hash, node_count);
	TESTING_ADD_VALUE_TO_HASH(input_hash, seed);
	TESTING_HASH_T output_hash = TESTING_STARTING_HASH;
	TESTING_ADD_DATA_TO_HASH(output_hash, supporting_data, sizeof(struct SUPPORTING_DATA_T));
	testing_assert_determinism("travelling_salesman_new_problem_determinism", input_hash, output_hash);


	return problem;
}

