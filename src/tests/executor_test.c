#include "types.h"
#include "testing.h"
#include "executors/executor.h"
#include "problems/x86/common.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

U8 payload[] = {
	0xB8, 0x00, 0x00, 0x00, 0x00,
	0xC6, 0x40, 0x00, 'H',
	0xC6, 0x40, 0x01, 'e',
	0xC6, 0x40, 0x02, 'l',
	0xC6, 0x40, 0x03, 'l',
	0xC6, 0x40, 0x04, 'o',
	0xC6, 0x40, 0x05, '!',
};

#define IO_DATA_LEN 0x1000
#define PROG_DATA_LEN 0x1000
#define END_SLED_LEN 0x1000

struct ml86_process_memory
{
	unsigned char io_data[IO_DATA_LEN];
	unsigned char program_data[PROG_DATA_LEN];
	unsigned char end_sled[END_SLED_LEN];
};

#define PROG_LEN 0x200

int main(int argc, const char *const *argv)
{
	seed_fast_rand(15);

	testing_initialize();

	static struct EXECUTOR_DATA_T executor_data;

	executor_init(&executor_data, sizeof(struct ml86_process_memory));
	struct ml86_process_memory * proc_mem = (struct ml86_process_memory *)EXECUTOR_PROCESS_MEM(&executor_data);

	// Try "Hello world" a lot

	for (int i = 0; i < 10000; i++)
	{
		memset(proc_mem->io_data, 0, IO_DATA_LEN);
		memset(proc_mem->program_data, 0x90, PROG_DATA_LEN);
		memset(proc_mem->end_sled, 0xf4, END_SLED_LEN);
		memcpy(proc_mem->program_data, payload, sizeof(payload));
		int did_hang = executor_execute(&executor_data, PROG_DATA_LEN);
		testing_assert("executor_test_1_did_hang", !did_hang);
		testing_assert("executor_test_1_did_print", proc_mem->io_data[0] == 'H');
	}

	// Fuzz and run hello world, look for code that forces the executor to bug out on successive runs

	for (int i = 0; i < 100000; i++)
	{
		// Fuzz
		memset(proc_mem->io_data, 0, IO_DATA_LEN);
		memset(proc_mem->program_data, 0x90, PROG_DATA_LEN);
		memset(proc_mem->end_sled, 0xf4, END_SLED_LEN);
		// Dummy problem
		struct Problem_T dummy_problem;
		dummy_problem.data_len = PROG_LEN;
		for (int j = 0; j < 1000; j++)
		{
			x86_basic_scramble(&dummy_problem, proc_mem->program_data);
		}
		U8 rand_prog_data[PROG_LEN];
		memcpy(rand_prog_data, proc_mem->program_data, PROG_LEN);
		U32 csum = qhashmurmur3_32(rand_prog_data, PROG_LEN);

		executor_execute(&executor_data, IO_DATA_LEN);

		// Hello world
		memset(proc_mem->io_data, 0, IO_DATA_LEN);
		memset(proc_mem->program_data, 0x90, PROG_DATA_LEN);
		memset(proc_mem->end_sled, 0xf4, END_SLED_LEN);
		memcpy(proc_mem->program_data, payload, sizeof(payload));
		int did_hang = executor_execute(&executor_data, PROG_DATA_LEN);
		testing_assert("executor_test_2_did_hang", !did_hang);
		if (!testing_assert("executor_test_2_did_print", proc_mem->io_data[0] == 'H'))
		{
			printf("Checksum: %x\n", csum);
			printf("Program: \n");
			print_data_as_hex(rand_prog_data, PROG_LEN);
		}

	}

}
