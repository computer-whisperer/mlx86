#include <cstdlib>
#include <cstdio>
#include <ProblemTravellingSalesman.h>
#include <SolverSimulatedAnnealing.h>
//#include <ProblemBARBuildOrder.hpp>
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

  //auto problem = new ProblemBARBuildOrder();
	//auto problem = new ProblemTravellingSalesman(1000, 10);
	//auto problem = new problem_audio_regen("../audio/582986__queenoyster__low-battery.wav");
	auto problem = new SolverHybridX86(512);
	//auto problem = new ProblemX86StringMatch("Hello!", 200);
	//auto problem = new ProblemHelloWorld("Hello!");

	auto solver = new SolverParallelTempering();
	//auto solver = new SolverParallelGreedy();
	//auto solver = new SolverSimulatedAnnealing();
	//auto solver = new SolverSimpleGreedy();
	//auto solver = new SolverTabuSearch();
	//auto solver = new SolverHybridX86("../text_solver_best");
	//solver->prettyPrintData(solver->code);
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
/*
	U8 buffer[problem->data_len];
	FILE * fpi = fopen("best_data", "rb");
	size_t len = fread(buffer, sizeof(U8), sizeof(buffer), fpi);
	fclose(fpi);
  problem->presentSolution(buffer);*/


  struct SolverResults_T results{};

	init_reporter_process(reporter_mem, (Problem*)problem);
	solver->run((Problem*)problem, reporter_mem, 100, 1000000, &results);
	deinit_reporter_process(reporter_mem);

  uint8_t first_pass_data[problem->data_len];
  memcpy(first_pass_data, results.data, problem->data_len);
/*
  auto final_solver = new SolverSimpleGreedy();
  init_reporter_process(reporter_mem, (Problem*)problem);
  final_solver->run((Problem*)problem, reporter_mem, 100, 100000, &results, first_pass_data);
  deinit_reporter_process(reporter_mem);
*/
	{
	    std::vector<double> scores;
	    for (uint32_t i = 0; i < 100; i++)
	    {
	        scores.push_back(problem->scalarTrial(results.data));
	    }
	    print_mean_and_stddev(scores);
	}

  problem->prettyPrintData(results.data);


	const char * output_filename = "best_data";
	FILE * fp = fopen(output_filename, "wb");
	fwrite(results.data, sizeof(U8), ((Problem*)problem)->data_len, fp);
	printf("Wrote %ld bytes to %s\r\n", problem->data_len, output_filename);
	fclose(fp);

	//((problem_audio_regen*)problem)->export_wav("test.wav", results.data);

	return 0;
}
