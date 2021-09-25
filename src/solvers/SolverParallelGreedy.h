//
// Created by christian on 9/10/21.
//

#ifndef MLX86_SOLVERPARALLELGREEDY_H
#define MLX86_SOLVERPARALLELGREEDY_H
#include "Solver.h"
class SolverParallelGreedy : public Solver
        {
    U32 cycle_len;
    U32 recheck_rate;
    U32 neighbor_post_rate;
    U32 neighbor_poll_rate;
    U32 neighbor_poll_chance;
    U32 num_neighbors;
        public:
            inline SolverParallelGreedy() :
            cycle_len(1000000),
            recheck_rate(10),
            neighbor_post_rate(10),
            neighbor_poll_rate(100),
            neighbor_poll_chance(10),
            num_neighbors(10)
            {};

            void tempering_process(Problem *problem, struct sync_mem_t * our_sync, struct sync_mem_t ** neighbor_syncs, U8* should_exit, struct reporting_mem_t * reporting_mem);

            void run(Problem *problem, struct REPORTER_MEM_T * reporter_mem, double score_limit, U32 trial_limit, struct SolverResults_T * results_out) override;
        };


#endif //MLX86_SOLVERPARALLELGREEDY_H
