#include <string.h>
#include "executors/executor.h"
#include "problems/problems.h"
#include "problems/x86/common.h"
#include "types.h"

#define IO_DATA_LEN 0x1000
#define PROG_DATA_LEN 0x1000
#define END_SLED_LEN 0x1000

struct ml86_process_memory
{
	unsigned char io_data[IO_DATA_LEN];
	unsigned char program_data[PROG_DATA_LEN];
	unsigned char end_sled[END_SLED_LEN];
};

static int executor_initialized = 0;
static struct EXECUTOR_DATA_T executor_data;
static struct ml86_process_memory * proc_mem;

float x86_scalar_trial_simple_meta(struct Problem_T * problem, U8* data)
{

	if (!executor_initialized)
	{
		executor_initialized = 1;
		executor_init(&executor_data, sizeof(struct ml86_process_memory));
	}

	proc_mem = EXECUTOR_PROCESS_MEM(&executor_data);
	memset(proc_mem->io_data, 0, IO_DATA_LEN);
	memset(proc_mem->program_data, 0x90, PROG_DATA_LEN);
	memset(proc_mem->end_sled, 0xf4, END_SLED_LEN);
	memcpy(proc_mem->program_data, data, problem->data_len);
	int did_hang = executor_execute(&executor_data, PROG_DATA_LEN);

	return problem->meta_problem->scalar_trial(problem->meta_problem, proc_mem->io_data) - 0.3*did_hang;
}


struct Problem_T problem_x86_hello_world =
{
	x86_scalar_trial_simple_meta,
	x86_basic_scramble,
	x86_pretty_print,
	x86_data_init,
	0x400,
	&problem_hello_world
};

struct Problem_T problem_x86_nonzero =
{
	x86_scalar_trial_simple_meta,
	x86_basic_scramble,
	x86_pretty_print,
	x86_data_init,
	0x200,
	&problem_nonzero
};
