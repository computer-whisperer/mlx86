#ifndef SOLVERS_H
#define SOLVERS_H
#include "reporters/reporter.h"
#include "problems/problem.h"
#include "types.h"


struct SolverResults_T
{
	F64 score;
	U64 trial_count;
	U8 * data;
};

class Solver
{
public:
	virtual void run(Problem *problem, struct REPORTER_MEM_T * reporter_mem, double score_limit, U32 trial_limit, struct SolverResults_T * results_out) {

    }
};

#endif
