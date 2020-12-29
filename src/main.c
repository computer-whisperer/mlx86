#include <stdio.h>
#include <stdlib.h>
#include "problems/problems.h"
#include "solvers/solvers.h"

int main(int argc, char * argv[])
{
	struct Problem_T * problem;
	//problem = &problem_nonzero;
	//problem = &problem_hello_world;
	problem = &problem_x86_calculator;
	//problem = &problem_x86_nonzero;
	//problem = &problem_x86_hello_world;
	//problem = &problem_starcraft2_supplymax;

	U8 * data = parallel_tempering(problem);
	//U8 * data = simulated_annealing(problem);

	FILE * fp = fopen("best_data", "wb");
	fwrite(data, sizeof(U8), problem->data_len, fp);
	fclose(fp);
	return 0;
}
