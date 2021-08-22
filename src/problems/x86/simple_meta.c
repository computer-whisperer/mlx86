#include <string.h>
#include "executors/executor.h"
#include "problems/problems.h"
#include "problems/x86/common.h"
#include "types.h"
#include "utils.h"

static struct EXECUTOR_DATA_T executor_data;

static void scalar_trial_init(struct Problem_T * problem)
{
  executor_init(&executor_data, problem->mesa_problem->data_len, problem->data_len);
}

static void scalar_trial_deinit(struct Problem_T * problem)
{
  executor_deinit(&executor_data);
}

float x86_scalar_trial_simple_meta(struct Problem_T * problem, U8* data)
{
	U8 * program_mem = (struct ml86_process_memory *)EXECUTOR_PROGRAM_MEM(&executor_data);
	U8 * io_mem = (struct ml86_process_memory *)EXECUTOR_IO_MEM(&executor_data);

	memset(io_mem, 0, problem->mesa_problem->data_len);
	memcpy(program_mem, data, problem->data_len);
	int did_hang = executor_execute(&executor_data);

	return problem->mesa_problem->scalar_trial(problem->mesa_problem, io_mem) - 0.3*did_hang;
}


struct Problem_T problem_x86_hello_world =
{
  scalar_trial_init,
  scalar_trial_deinit,
	x86_scalar_trial_simple_meta,
	x86_basic_scramble,
	x86_pretty_print,
	x86_data_init,
	0x400,
	&problem_hello_world
};

struct Problem_T problem_x86_nonzero =
{
  scalar_trial_init,
  scalar_trial_deinit,
	x86_scalar_trial_simple_meta,
	x86_basic_scramble,
	x86_pretty_print,
	x86_data_init,
	0x200,
	&problem_nonzero
};
