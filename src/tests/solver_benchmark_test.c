#include "utils.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "problems/problem.h"
#include "solvers/Solver.h"
#include "testing.h"

#define NUM_PROBLEMS_PER_POINT 10

int main(int argc, char * argv[])
{
	testing_initialize();


	U8 buffer[0x1000];
	FILE * fp = fopen("tsp_hybrid_x86", "rb");
	size_t len = fread(buffer, sizeof(U8), sizeof(buffer), fp);
	fclose(fp);
	struct Solver_T * hybrid_x86 = build_solver_hybrid_x86(buffer, len);

	struct Solver_T * solvers[] =
	{
			&solver_simple_greedy,
			&solver_simulated_annealing,
			&solver_tabu_search,
			hybrid_x86
	};

	struct Problem_T * problems[NUM_PROBLEMS_PER_POINT];
	for (U32 p = 0; p < NUM_PROBLEMS_PER_POINT; p++)
	{
		problems[p] = travelling_salesman_new_problem(100, p);
	}


	for (U32 i = 100; i < 100000; i*=1.2)
	{
		printf("%d, ", i);
		for (U32 s = 0; s < (sizeof(solvers)/sizeof(solvers[0])); s++)
		{
			F64 score = 0;
			for (U32 p = 0; p < NUM_PROBLEMS_PER_POINT; p++)
			{
				struct SolverResults_T results;
				seed_fast_rand(10);
				solvers[s]->solver(solvers[s], problems[p], NULL, 100, i, &results);
				score += results.score/(F64)NUM_PROBLEMS_PER_POINT;
				free(results.data);
			}
			printf("%g, ", score);
		}
		printf("\n");
		fflush(0);
	}
}
