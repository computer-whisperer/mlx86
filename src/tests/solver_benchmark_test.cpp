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

	U8 buffer[0x1000];
	FILE * fp = fopen("../best_data", "rb");
	size_t len = fread(buffer, sizeof(U8), sizeof(buffer), fp);
	fclose(fp);
	auto hybrid_x86 = new SolverHybridX86(len);
	hybrid_x86->code = buffer;

	Solver * solvers[] = {
		new SolverSimpleGreedy(),
		new SolverSimulatedAnnealing(),
		hybrid_x86
	};

	//Problem * problems[NUM_PROBLEMS_PER_POINT];
	//for (U32 p = 0; p < NUM_PROBLEMS_PER_POINT; p++)
	//{
	//	problems[p] = new ProblemTravellingSalesman(100, p);
	//}
	Problem * problems[] = {
		new ProblemHelloWorld(),
		new ProblemHelloWorld("Fuck!"),
		new ProblemHelloWorld("Inception!")
	};


	for (U32 i = 10; i < 100000; i+= 100)
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
