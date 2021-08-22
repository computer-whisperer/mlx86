#include "problems/problems.h"
#include "executors/executor.h"
#include "utils.h"
#include "problems/x86/common.h"
#include <string.h>

struct io_data_t
{
  U8 a;
  U8 b;
  U8 op;
  U8 o;
  U8 scratch[40];
};

static struct EXECUTOR_DATA_T executor_data;

static void scalar_trial_init(struct Problem_T * problem)
{
  executor_init(&executor_data, sizeof(struct io_data_t), problem->data_len);
}

static void scalar_trial_deinit(struct Problem_T * problem)
{
  executor_deinit(&executor_data);
}




#define NUM_TESTS 40

float x86_scalar_trial_calculator(struct Problem_T * problem, U8 * data)
{
	U8 * program_mem = (struct ml86_process_memory *)EXECUTOR_PROGRAM_MEM(&executor_data);
	struct io_data_t * io_mem = (struct ml86_process_memory *)EXECUTOR_IO_MEM(&executor_data);

	memset(io_mem, 0, sizeof(struct io_data_t));
	memcpy(program_mem, data, problem->data_len);
  
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
    io_mem->a = a;
    io_mem->b = b;
    io_mem->op = op;
    io_mem->o = 0;
    int did_hang = executor_execute(&executor_data);
    error += did_hang*0.3;
    
    double local_error = io_mem->o - o;
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

