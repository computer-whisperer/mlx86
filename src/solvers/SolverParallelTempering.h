//
// Created by christian on 9/10/21.
//

#ifndef MLX86_SOLVERPARALLELTEMPERING_H
#define MLX86_SOLVERPARALLELTEMPERING_H
#include "Solver.h"
class SolverParallelTempering : public Solver
        {
    U32 cycle_len;
    U32 recheck_rate;
    F64 score_diff_multiplier;
    U32 neighbor_post_rate;
    U32 neighbor_poll_rate;
    U32 neighbor_poll_chance;
    F64 score_diff_neighbor_multiplier;
    U32 num_neighbors;
        public:
            inline SolverParallelTempering() :
            cycle_len(5000),
            recheck_rate(10),
            score_diff_multiplier(10000.0),
            neighbor_post_rate(10),
            neighbor_poll_rate(500),
            neighbor_poll_chance(10),
            score_diff_neighbor_multiplier(10000.0),
            num_neighbors(10)
            {};

            void tempering_process(Problem *problem, struct sync_mem_t * our_sync, sync_mem_t **neighbor_syncs, U8* should_exit, struct reporting_mem_t * reporting_mem);

            void run(Problem *problem, struct REPORTER_MEM_T * reporter_mem, double score_limit, U32 trial_limit, struct SolverResults_T * results_out, uint8_t* starting_data=nullptr) override;
        };


#endif //MLX86_SOLVERPARALLELTEMPERING_H
