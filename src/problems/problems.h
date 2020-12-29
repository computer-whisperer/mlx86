#ifndef PROBLEMS_PROBLEMS_H
#define PROBLEMS_PROBLEMS_H
#include <stddef.h>
#include "types.h"

struct Problem_T
{
	float (*scalar_trial)(struct Problem_T * problem, U8* data);
	void (*scrambler)(struct Problem_T * problem, U8* data);
	void (*data_pretty_printer)(struct Problem_T * problem, U8* data);
	void (*data_initializer)(struct Problem_T * problem, U8* data);
	size_t data_len;
	struct Problem_T * meta_problem;
};

extern struct Problem_T problem_nonzero;
extern struct Problem_T problem_hello_world;
extern struct Problem_T problem_x86_calculator;
extern struct Problem_T problem_x86_nonzero;
extern struct Problem_T problem_x86_hello_world;
extern struct Problem_T problem_starcraft2_supplymax;

#endif
