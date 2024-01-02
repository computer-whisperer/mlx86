#include <christian_utils.h>

#include <string.h>
#include <math.h>
#include <ProblemHelloWorld.h>
#include <ProblemTravellingSalesman.h>
#include <ProblemX86StringMatch.h>
#include <SolverHybridX86.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "problems/problem.h"
#include "testing.h"
#include "SolverSimpleGreedy.h"
#include "SolverSimulatedAnnealing.h"
#include "SolverTabuSearch.h"

#define NUM_PROBLEMS_PER_POINT 3

int main(int argc, char * argv[])
{
	testing_initialize();

	Solver * solvers[] = {
		new SolverSimpleGreedy(),
		new SolverSimulatedAnnealing(),
		new SolverHybridX86("../best_data")
	};

	//Problem * problems[NUM_PROBLEMS_PER_POINT];
	//for (U32 p = 0; p < NUM_PROBLEMS_PER_POINT; p++)
	//{
	//	problems[p] = new ProblemTravellingSalesman(100, p);
	//}
	Problem * problems[] = {
		new ProblemHelloWorld("Hello there Obi-Wan Kenobi!!!"),
		new ProblemHelloWorld("I am mlx86!!!"),
		new ProblemHelloWorld("Hello World!!!")
	};


	for (U32 i = 10; i < 5000; i+= 10)
	{
		printf("%d, ", i);
		for (U32 s = 0; s < std::size(solvers); s++)
		{
			F64 score = 0;
			for (U32 p = 0; p < std::size(problems); p++)
			{
				struct SolverResults_T results;
				seed_fast_rand(10);
				solvers[s]->run(problems[p], NULL, 100, i, &results, nullptr);
				score += results.score/(F64)std::size(problems);
				free(results.data);
			}
			printf("%g, ", score);
		}
		printf("\n");
		fflush(0);
	}
}
