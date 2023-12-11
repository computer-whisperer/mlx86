#include <christian_utils.h>

#include <string.h>
#include <math.h>
#include <ProblemTravellingSalesman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "problems/problem.h"
#include "testing.h"
#include "SolverSimpleGreedy.h"
#include "SolverSimulatedAnnealing.h"
#include "SolverTabuSearch.h"

#define NUM_PROBLEMS_PER_POINT 10

int main(int argc, char * argv[])
{
	testing_initialize();

	Solver * solvers[] = {
		new SolverSimpleGreedy(),
		new SolverSimulatedAnnealing(),
		new SolverTabuSearch()
	};

	Problem * problems[NUM_PROBLEMS_PER_POINT];
	for (U32 p = 0; p < NUM_PROBLEMS_PER_POINT; p++)
	{
		problems[p] = new ProblemTravellingSalesman(100, p);
	}


	for (U32 i = 100; i < 1000000; i*=1.2)
	{
		printf("%d, ", i);
		for (U32 s = 0; s < (sizeof(solvers)/sizeof(solvers[0])); s++)
		{
			F64 score = 0;
			for (U32 p = 0; p < NUM_PROBLEMS_PER_POINT; p++)
			{
				struct SolverResults_T results;
				seed_fast_rand(10);
				solvers[s]->run(problems[p], NULL, 100, i, &results, nullptr);
				score += results.score/(F64)NUM_PROBLEMS_PER_POINT;
				free(results.data);
			}
			printf("%g, ", score);
		}
		printf("\n");
		fflush(0);
	}
}
