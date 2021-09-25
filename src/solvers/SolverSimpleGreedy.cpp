#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include "solvers/Solver.h"
#include "problems/problem.h"
#include "reporters/reporter.h"
#include "types.h"
#include "christian_utils.h"
#include "SolverSimpleGreedy.h"

void SolverSimpleGreedy::run(Problem *problem, struct REPORTER_MEM_T *reporter_mem, double score_limit, U32 trial_limit,
                             struct SolverResults_T *results_out) {

    U8 *data = static_cast<U8 *>(malloc(problem->data_len));
    U8 *data_b = static_cast<U8 *>(malloc(problem->data_len));
    problem->dataInit(data);
    memcpy(data_b, data, problem->data_len);

    double score = problem->scalarTrial(data);

    if (reporter_mem) {
        reporter_mem->mtx.lock();
        memcpy(reporter_mem->current_data, data, problem->data_len);
        reporter_mem->mtx.unlock();
    }

    U64 total_tests = 0;
    while (true) {
        // Rescore occasionally
        if ((total_tests % 10) == 0) {
            score = score * 0.5 + problem->scalarTrial(data) * 0.5;
        }

        // Copy data to data_b and modify it
        memcpy(data_b, data, problem->data_len);


        /* Make a random change and introduce a new agent */
        problem->scrambler(data);

        // Score run
        double new_score = problem->scalarTrial(data);
        total_tests++;

        // Simulated annealing to determine if this passes
        if (new_score >= score) {
            // Keep the new code
            score = new_score;
        } else {
            // Revert to the old code
            U8 *t = data_b;
            data_b = data;
            data = t;
        }
        if (reporter_mem) {
            if (!(total_tests % 10)) {
                reporter_mem->mtx.lock();
                memcpy(reporter_mem->current_data, data, problem->data_len);
                reporter_mem->trials_completed = total_tests;
                reporter_mem->mtx.unlock();
            }
            if (reporter_mem->abort_solve) {
                break;
            }
        }

        if (score >= score_limit) {
            break;
        }
        if (total_tests >= trial_limit) {
            break;
        }
    }

    if (results_out) {
        results_out->data = static_cast<U8 *>(malloc(problem->data_len));
        memcpy(results_out->data, data, problem->data_len);
        results_out->score = score;
        results_out->trial_count = total_tests;
    }
    free(data);
    free(data_b);
}