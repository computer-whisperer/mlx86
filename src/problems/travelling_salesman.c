#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "problems/problems.h"
#include "problems/common.h"
#include "utils.h"
#include "types.h"
#include "testing.h"

#define MAX_NODE_COUNT 256

struct SUPPORTING_DATA_T
{
	U32 node_count;
	U32 seed;
	double x_coords[MAX_NODE_COUNT];
	double y_coords[MAX_NODE_COUNT];
};

static float scalar_trial(struct Problem_T * problem, U8 * data)
{
	struct SUPPORTING_DATA_T * supporting_data = (struct SUPPORTING_DATA_T *)problem->supporting_data;

	U32 l = supporting_data->node_count;
	if (problem->data_len < l)
	{
		l = problem->data_len;
	}

	testing_assert("travelling_salesman_scalar_trial_1", l > 0);

	U8 state_track[MAX_NODE_COUNT];
	memset(state_track, 0, l);
	U32 num_states_visited = 1;

	double len = 0;
	U32 last_state = data[0]%l;
	for (U32 i = 1; i < l; i++)
	{
		U32 next_state = data[i]%l;

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

	float cost = (len + (l - num_states_visited))/l;

	// Determinism test
	TESTING_HASH_T input_hash = TESTING_STARTING_HASH;
	TESTING_ADD_DATA_TO_HASH(input_hash, supporting_data, sizeof(struct SUPPORTING_DATA_T));
	TESTING_ADD_DATA_TO_HASH(input_hash, data, problem->data_len);
	TESTING_HASH_T output_hash = TESTING_STARTING_HASH;
	TESTING_ADD_DATA_TO_HASH(output_hash, supporting_data, sizeof(struct SUPPORTING_DATA_T));
	testing_assert_determinism("travelling_salesman_scalar_trial_determinism", input_hash, output_hash);


	return cost;
}

static void scramble(struct Problem_T * problem, U8 * data)
{
	U8 a = fast_rand()%problem->data_len;
	U8 b = fast_rand()%problem->data_len;
	U8 c = data[a];
	data[a] = data[b];
	data[b] = c;
}

static void data_init(struct Problem_T * problem, U8 * data)
{
	struct SUPPORTING_DATA_T * supporting_data = (struct SUPPORTING_DATA_T *)problem->supporting_data;

	U32 l = supporting_data->node_count;
	if (problem->data_len < l)
		l = problem->data_len;

	for (U32 i = 0; i < l; i++)
	{
		data[i] = i;
	}
}

struct Problem_T * travelling_salesman_new_problem(U8 node_count, U32 seed)
{
	struct Problem_T * problem = malloc(sizeof(struct Problem_T));
	problem->trial_initializer = NULL;
	problem->trial_deinitializer = NULL;
	problem->scalar_trial = scalar_trial;
	problem->scrambler = scramble;
	problem->data_pretty_printer = hex_pretty_print;
	problem->data_initializer = data_init;
	problem->data_len = node_count;
	problem->meta_problem = NULL;
	struct SUPPORTING_DATA_T * supporting_data = malloc(sizeof(struct SUPPORTING_DATA_T));
	problem->supporting_data = supporting_data;

	// Swap out seed
	U32 prev_seed = get_fast_rand_seed();
	seed_fast_rand(seed);


	supporting_data->seed = seed;
	supporting_data->node_count = node_count;

	for (U32 i = 0; i < node_count; i++)
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

