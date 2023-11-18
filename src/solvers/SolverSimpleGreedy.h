//
// Created by christian on 9/10/21.
//

#ifndef MLX86_SOLVERSIMPLEGREEDY_H
#define MLX86_SOLVERSIMPLEGREEDY_H
#include "Solver.h"
class SolverSimpleGreedy : public Solver
        {
        public:

            void run(Problem *problem, struct REPORTER_MEM_T * reporter_mem, double score_limit, U32 trial_limit, struct SolverResults_T * results_out, uint8_t* starting_data=nullptr) override;
        };


#endif //MLX86_SOLVERSIMPLEGREEDY_H
