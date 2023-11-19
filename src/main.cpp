#include <cstdlib>
#include <cstdio>
#include <ProblemTravellingSalesman.h>
#include <SolverSimulatedAnnealing.h>
#include <ProblemBARBuildOrder.hpp>
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
#include <vector>
#include <complex>
#include <iostream>
#include <SolverTabuSearch.h>
#include <cstring>

void print_mean_and_stddev(const std::vector<double>& values)
{
    double mean = 0;
    for (auto & i : values)
    {
        mean += i;
    }
    mean = mean / (double)values.size();

    double variance = 0;
    for (auto & i : values)
    {
        double v = i - mean;
        variance += v*v;
    }
    variance = variance / (double)values.size();
    double standard_deviation = std::sqrt(variance);
    std::cout << "Mean: " << mean << std::endl;
    std::cout << "Std Deviation: " << standard_deviation << std::endl;
}

int main(int argc, char * argv[])
{
	testing_initialize();
	zydis_init();

	struct REPORTER_MEM_T * reporter_mem = reporter_mem_init();

  auto problem = new ProblemBARBuildOrder();
	//auto problem = new ProblemTravellingSalesman(1000, 10);
	//auto problem = new problem_audio_regen("../audio/582986__queenoyster__low-battery.wav");
	//auto problem = new SolverHybridX86(800);
	//auto problem = new ProblemX86StringMatch("I am a creature of mystery", 800);
	//auto problem = new ProblemHelloWorld();

	auto solver = new SolverParallelTempering();
	//auto solver = new SolverParallelGreedy();
	//auto solver = new SolverSimulatedAnnealing();
	//auto solver = new SolverSimpleGreedy();
	//auto solver = new SolverTabuSearch();
/*
	{
	    std::cout << "Solver score:" << std::endl;
        std::vector<double> scores;
        for (uint32_t i = 0; i < 100; i++)
        {
            scores.push_back(ProblemTravellingSalesman::SolverTest(solver));
        }
        print_mean_and_stddev(scores);
	}
*/
	//U8 buffer[0x1000];
	//FILE * fp = fopen("tsp_hybrid_x86", "rb");
	//size_t len = fread(buffer, sizeof(U8), sizeof(buffer), fp);
	//fclose(fp);

	struct SolverResults_T results{};

	init_reporter_process(reporter_mem, (Problem*)problem);
	solver->run((Problem*)problem, reporter_mem, 100, 1000*60*1, &results);
	deinit_reporter_process(reporter_mem);

  uint8_t first_pass_data[problem->data_len];
  memcpy(first_pass_data, results.data, problem->data_len);

  auto final_solver = new SolverSimpleGreedy();
  init_reporter_process(reporter_mem, (Problem*)problem);
  final_solver->run((Problem*)problem, reporter_mem, 100, 10000, &results, first_pass_data);
  deinit_reporter_process(reporter_mem);

	{
	    std::vector<double> scores;
	    for (uint32_t i = 0; i < 100; i++)
	    {
	        scores.push_back(problem->scalarTrial(results.data));
	    }
	    print_mean_and_stddev(scores);
	}

  problem->prettyPrintData(results.data);


	//FILE * fp = fopen("best_data", "wb");
	//fwrite(results.data, sizeof(U8), ((Problem*)problem)->data_len, fp);
	//fclose(fp);

	//((problem_audio_regen*)problem)->export_wav("test.wav", results.data);

	return 0;
}
