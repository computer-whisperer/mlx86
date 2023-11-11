#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cstring>
#include "problems/problem.h"
#include "christian_utils.h"
#include "types.h"
#include "testing.h"
#include "ProblemTravellingSalesman.h"



double ProblemTravellingSalesman::scalarTrial(U8 *data) {

    auto * steps = (state_t *)data;

    testing_assert("travelling_salesman_scalar_trial_1", node_count > 0);

    U8 state_track[node_count];
    memset(state_track, 0, node_count);
    U32 num_states_visited = 1;

    double len = 0;
    state_t last_state = steps[0];
    if (last_state > node_count)
    {
        last_state = 0;
    }
    for (state_t i = 1; i < node_count; i++)
    {
        state_t next_state = steps[i];
        if (next_state >= node_count)
        {
            next_state = 0;
        }

        if (!state_track[next_state])
        {
            num_states_visited++;
        }

        state_track[next_state] = 1;

        double x_diff = x_coords[next_state] - x_coords[last_state];
        double y_diff = y_coords[next_state] - y_coords[last_state];
        double dist = sqrt(x_diff*x_diff + y_diff*y_diff);
        len += dist;
        last_state = next_state;
    }

    testing_assert("travelling_salesman_scalar_trial_2", len > 0);

    float cost = (len + (node_count - num_states_visited))/node_count;

    // Determinism test
    /*
    TESTING_HASH_T input_hash = TESTING_STARTING_HASH;
    TESTING_ADD_DATA_TO_HASH(input_hash, supporting_data, sizeof(struct SUPPORTING_DATA_T));
    TESTING_ADD_DATA_TO_HASH(input_hash, data, problem->data_len);
    TESTING_HASH_T output_hash = TESTING_STARTING_HASH;
    TESTING_ADD_DATA_TO_HASH(output_hash, supporting_data, sizeof(struct SUPPORTING_DATA_T));
    testing_assert_determinism("travelling_salesman_scalar_trial_determinism", input_hash, output_hash);
*/

    return 1-cost;
}

void ProblemTravellingSalesman::scrambler(U8 *data) {
    auto * steps = (state_t *)data;
    state_t a = fast_rand()%node_count;
    state_t b = fast_rand()%node_count;
    state_t c = steps[a];
    steps[a] = steps[b];
    steps[b] = c;
}

void ProblemTravellingSalesman::prettyPrintData(U8 *data) {
    size_t len = data_len;
    if (len > 100)
        len = 100;
    print_data_as_hex(data, len);
}

ProblemTravellingSalesman::ProblemTravellingSalesman(int node_count_in, int seed_in):
node_count(node_count_in),
seed(seed_in),
Problem(node_count_in*sizeof(state_t))
{
    // Swap out seed
    int prev_seed = get_fast_rand_seed();
    seed_fast_rand(seed);

    for (state_t i = 0; i < node_count; i++)
    {
        x_coords[i] = (double)fast_rand()/(double)FAST_RAND_MAX;
        y_coords[i] = (double)fast_rand()/(double)FAST_RAND_MAX;
    }

    // Restore old seed
    seed_fast_rand(prev_seed);

    // Determinism test
    /*
    TESTING_HASH_T input_hash = TESTING_STARTING_HASH;
    TESTING_ADD_VALUE_TO_HASH(input_hash, node_count);
    TESTING_ADD_VALUE_TO_HASH(input_hash, seed);
    TESTING_HASH_T output_hash = TESTING_STARTING_HASH;
    TESTING_ADD_DATA_TO_HASH(output_hash, supporting_data, sizeof(struct SUPPORTING_DATA_T));
    testing_assert_determinism("travelling_salesman_new_problem_determinism", input_hash, output_hash);
*/
}

void ProblemTravellingSalesman::dataInit(U8 *data) {
    auto * steps = (state_t *)data;
    struct SUPPORTING_DATA_T * supporting_data = (struct SUPPORTING_DATA_T *)supporting_data;

    state_t l = node_count;

    for (state_t i = 0; i < l; i++)
    {
        steps[i] = i;
    }
}

double ProblemTravellingSalesman::SolverTest(Solver *solver, int node_count, int solver_iterations, int problem_iterations) {
    struct SolverResults_T results{};
    double score = 0;
    for (int i = 0; i < problem_iterations; i++)
    {
        auto training_problem = new ProblemTravellingSalesman(node_count, i);
        solver->run(training_problem, nullptr, 1, solver_iterations, &results);
        free(results.data);
        score += results.score;
    }
    return score/(double)problem_iterations;
}

double ProblemTravellingSalesman::SolverTest(Solver *solver) {
    return SolverTest(solver, 20, 100, 2);
}
