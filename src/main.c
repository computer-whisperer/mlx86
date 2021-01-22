#include <stdio.h>
#include <stdlib.h>
#include "problems/problems.h"
#include "solvers/solvers.h"
#include "testing.h"

int main(int argc, char * argv[])
{
	testing_initialize();

	struct REPORTER_MEM_T * reporter_mem = reporter_mem_init();

	struct Problem_T * problem;
	//problem = &problem_nonzero;
	//problem = &problem_hello_world;
	//problem = &problem_x86_calculator;
	//problem = &problem_x86_nonzero;
	problem = &problem_x86_hello_world;
	//problem = &problem_starcraft2_supplymax;
	//problem = &problem_simulated_annealing_hyperparameters;
    //problem = &problem_parallel_tempering_hyperparameters;
	//problem->meta_problem = travelling_salesman_new_problem(20, 24);
	//problem = travelling_salesman_new_problem(255, 24);

	const struct SimulatedAnnealing_Hyperparameters_T simulated_annealing_hyperparams = {
		1000000, // U32 first_cycle_len;
		10, // U32 recheck_rate;
		10000.0, // F64 score_diff_multiplier;
		1.5 // F64 cycle_multiplier
	};

	const struct ParallelTempering_Hyperparameters_T parallel_tempering_hyperparameters = {
		100000, // U32 cycle_len;
		10, // U32 recheck_rate;
		10000.0, // F64 score_diff_multiplier;
		2, // U32 neighbor_post_rate;
		10, // U32 neighbor_poll_rate;
		4, // U32 neighbor_poll_chance;
		10000.0, //F64 score_diff_neighbor_multiplier
		10 // U32 num_neighbors;
	};


	U8 * data = malloc(problem->data_len);
	init_reporter_process(reporter_mem, problem);
	//parallel_tempering(problem, &parallel_tempering_hyperparameters, reporter_mem, 1, 1000000, data, NULL, NULL);
	//simulated_annealing(problem, &simulated_annealing_hyperparams, reporter_mem, 2, simulated_annealing_hyperparams.first_cycle_len, data, NULL, NULL);
	//simple_greedy(problem, reporter_mem, 2, 10000000, data, NULL, NULL);
	tabu_search(problem, reporter_mem, 2, 10000000, data, NULL, NULL);

	deinit_reporter_process(reporter_mem);

	//FILE * fp = fopen("best_data", "wb");
	//fwrite(data, sizeof(U8), problem->data_len, fp);
	//fclose(fp);

	return 0;
}
