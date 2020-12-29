#ifndef SOLVERS_H
#define SOLVERS_H

#include "problems/problems.h"
#include "types.h"

U8* simulated_annealing(struct Problem_T * problem);
U8* parallel_tempering(struct Problem_T * problem);

#endif
