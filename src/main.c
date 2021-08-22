#include <stdio.h>
#include <stdlib.h>
#include "problems/problems.h"
#include "solvers/solvers.h"
#include "testing.h"
#include "zydis.h"

int main(int argc, char * argv[])
{
	testing_initialize();
	zydis_init();

	struct REPORTER_MEM_T * reporter_mem = reporter_mem_init();

	struct Problem_T * problem;
	//problem = &problem_nonzero;
	//problem = &problem_hello_world;
	//problem = &problem_x86_calculator;
	//problem = &problem_x86_nonzero;
	//problem = &problem_x86_hello_world;
	//problem = &problem_starcraft2_supplymax;
	//problem->meta_problem = travelling_salesman_new_problem(20, 24);
	problem = travelling_salesman_new_problem(100, 10);
	//problem = new_problem_hybrid_x86_hyperparameters();

	struct Solver_T * solver;
	//solver = &solver_parallel_tempering;
	//solver = &solver_parallel_tempering_slow;
	solver = &solver_simulated_annealing;
	//solver = &solver_simple_greedy;
	//solver = &solver_tabu_search;
	//solver = &build_solver_hybrid_x86();
	//U8 buffer[0x1000];
	//FILE * fp = fopen("tsp_hybrid_x86", "rb");
	//size_t len = fread(buffer, sizeof(U8), sizeof(buffer), fp);
	//fclose(fp);
	//solver = build_solver_hybrid_x86(buffer, len);

	struct SolverResults_T results;

	init_reporter_process(reporter_mem, problem);
	solver->solver(solver, problem, reporter_mem, 10, 1000000, &results);

	deinit_reporter_process(reporter_mem);

	FILE * fp = fopen("tsp_hybrid_x86", "wb");
	fwrite(results.data, sizeof(U8), problem->data_len, fp);
	fclose(fp);

	return 0;
}
