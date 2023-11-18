//
// Created by christian on 9/10/21.
//

#ifndef MLX86_SOLVERSIMULATEDANNEALING_H
#define MLX86_SOLVERSIMULATEDANNEALING_H
#include "problem.h"
#include "Solver.h"

class SolverSimulatedAnnealing : public Solver
{
    U32 first_cycle_len;
    U32 recheck_rate;
    F64 score_diff_multiplier;
    F64 cycle_multiplier;
public:
    inline SolverSimulatedAnnealing() :
        first_cycle_len(1000000),
        recheck_rate(10),
        score_diff_multiplier(10000.0),
        cycle_multiplier(1.5)
        {};

    void run(Problem *problem, struct REPORTER_MEM_T * reporter_mem, double score_limit, U32 trial_limit, struct SolverResults_T * results_out, uint8_t* starting_data=nullptr) override;
};

#endif //MLX86_SOLVERSIMULATEDANNEALING_H
