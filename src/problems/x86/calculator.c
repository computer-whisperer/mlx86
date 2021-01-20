#include "problems/problems.h"
#include "executors/executor.h"
#include "utils.h"
#include "problems/x86/common.h"
#include <string.h>

#define IO_DATA_LEN 0x1000
#define PROG_DATA_LEN 0x1000
#define END_SLED_LEN 0x1000

struct ml86_process_memory
{
	unsigned char io_data[IO_DATA_LEN];
	unsigned char program_data[PROG_DATA_LEN];
	unsigned char end_sled[END_SLED_LEN];
};

static struct EXECUTOR_DATA_T executor_data;
static struct ml86_process_memory * proc_mem;

static void scalar_trial_init(struct Problem_T * problem)
{
  executor_init(&executor_data, sizeof(struct ml86_process_memory));
}

static void scalar_trial_deinit(struct Problem_T * problem)
{
  executor_deinit(&executor_data, sizeof(struct ml86_process_memory));
}


struct prog_io
{
  U8 a;
  U8 b;
  U8 op;
  U8 o;
};

#define NUM_TESTS 40

float x86_scalar_trial_calculator(struct Problem_T * problem, U8 * data)
{

	proc_mem = EXECUTOR_PROCESS_MEM(&executor_data);
	memset(proc_mem->io_data, 0, IO_DATA_LEN);
	memset(proc_mem->program_data, 0x90, PROG_DATA_LEN);
	memset(proc_mem->end_sled, 0xf4, END_SLED_LEN);
	memcpy(proc_mem->program_data, data, problem->data_len);
  
  double error = 0;
  
  for (U32 i = 0; i < NUM_TESTS; i++)
  {
    U8 a = fast_rand_seeded(i+100)%100;
    U8 b = fast_rand_seeded(i+300)%100;
    U8 op = 0;//fast_rand_seeded(i+500)%2;
    U8 o = 0;
    switch (op)
    {
      case 0:
        o = a + b;
        break;
      case 1:
        o = a - b;
        break;
      case 2:
        o = a * b;
        break;
      case 3:
        o = a / b;
        break;
    }
    struct prog_io * io = (struct prog_io *)&(proc_mem->io_data);
    io->a = a;
    io->b = b;
    io->op = op;
    io->o = 0;
    int did_hang = executor_execute(&executor_data, PROG_DATA_LEN);
    error += did_hang*0.3;
    
    double local_error = io->o - o;
    if (local_error < 0)
    {
      local_error = -local_error;
    }
    if (local_error > 1000)
    {
      local_error = 1000;
    }
    error += local_error/1000.0;
    
  }

	return 1 - (error/NUM_TESTS);

}

struct Problem_T problem_x86_calculator =
{
    scalar_trial_init,
    scalar_trial_deinit,
	x86_scalar_trial_calculator,
	x86_basic_scramble,
	x86_pretty_print,
	x86_data_init,
	0x200,
	NULL
};

