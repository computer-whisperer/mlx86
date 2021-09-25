#include <cstdlib>
#include <cstdio>
#include <ProblemTravellingSalesman.h>
#include <SolverSimulatedAnnealing.h>
#include <problem_audio_regen.h>
#include <SolverSimpleGreedy.h>
#include <SolverParallelTempering.h>
#include <SolverParallelGreedy.h>
#include <SolverHybridX86.h>
#include <ProblemX86StringMatch.h>
#include <ProblemHelloWorld.h>
#include "problems/problem.h"
#include "solvers/Solver.h"
#include "testing.h"
#include "zydis.h"

int main(int argc, char * argv[])
{
	testing_initialize();
	zydis_init();

	struct REPORTER_MEM_T * reporter_mem = reporter_mem_init();

	//auto problem = new ProblemTravellingSalesman(1000, 10);
	//auto problem = new problem_audio_regen("../audio/582986__queenoyster__low-battery.wav");
	//auto problem = (Problem*)new SolverHybridX86(200);
	auto problem = new ProblemX86StringMatch("Hello", 200);
	//auto problem = new ProblemHelloWorld();

	auto solver = new SolverParallelTempering();
	//auto solver = new SolverParallelGreedy();
	//auto solver = new SolverSimulatedAnnealing();
	//auto solver = new SolverSimpleGreedy();

	//U8 buffer[0x1000];
	//FILE * fp = fopen("tsp_hybrid_x86", "rb");
	//size_t len = fread(buffer, sizeof(U8), sizeof(buffer), fp);
	//fclose(fp);
	//solver = build_solver_hybrid_x86(buffer, len);

	struct SolverResults_T results{};

	init_reporter_process(reporter_mem, problem);
	solver->run(problem, reporter_mem, 10, 1000000, &results);
	deinit_reporter_process(reporter_mem);

	double mean = 0;

	const int n = 100000;

	std::vector<double> scores;
	for (uint32_t i = 0; i < n; i++)
	{
	    scores.push_back(problem->scalarTrial(results.data));
	}

	for (auto & i : scores)
	{
	    mean += i;
	}
	mean = mean / n;

	double variance = 0;
	for (auto & i : scores)
	{
	    double v = i - mean;
	    variance += v*v;
	}
	variance = variance/n;
	double standard_deviation = std::sqrt(variance);
	std::cout << "Mean: " << mean << std::endl;
	std::cout << "Std Deviation: " << standard_deviation << std::endl;

	//((problem_audio_regen*)problem)->export_wav("test.wav", results.data);

	return 0;
}
