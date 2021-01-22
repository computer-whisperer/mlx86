#ifndef SOLVERS_H
#define SOLVERS_H
#include "reporters/reporter.h"
#include "problems/problems.h"
#include "types.h"

struct SimulatedAnnealing_Hyperparameters_T
{
	U32 first_cycle_len;
	U32 recheck_rate;
	F64 score_diff_multiplier;
	F64 cycle_multiplier;
};

struct ParallelTempering_Hyperparameters_T
{
	U32 cycle_len;
	U32 recheck_rate;
	F64 score_diff_multiplier;
	U32 neighbor_post_rate;
	U32 neighbor_poll_rate;
	U32 neighbor_poll_chance;
	F64 score_diff_neighbor_multiplier;
	U32 num_neighbors;
};

extern const struct SimulatedAnnealing_Hyperparameters_T simulated_annealing_default_hyperparameters;
extern const struct ParallelTempering_Hyperparameters_T parallel_tempering_default_hyperparameters;

void simulated_annealing(struct Problem_T * problem, const struct SimulatedAnnealing_Hyperparameters_T * hyperparameters, struct REPORTER_MEM_T * reporter_mem, double score_limit, U32 trial_limit, U8 * data_out, double * score_out, U32 * iterations_out);
void parallel_tempering(struct Problem_T * problem, const struct ParallelTempering_Hyperparameters_T * hyperparameters, struct REPORTER_MEM_T * reporter_mem, double score_limit, U32 trial_limit, U8 * data_out, double * score_out, U32 * iterations_out);
void simple_greedy(struct Problem_T * problem, struct REPORTER_MEM_T * reporter_mem, double score_limit, U32 trial_limit, U8 * data_out, double * score_out, U32 * iterations_out);
void tabu_search(struct Problem_T * problem, struct REPORTER_MEM_T * reporter_mem, double score_limit, U32 trial_limit, U8 * data_out, double * score_out, U32 * iterations_out);


#endif
