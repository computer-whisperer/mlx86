#include "types.h"
#include "testing.h"
#include "executors/executor.h"
#include "problems/x86/X86Common.h"
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

#define FUZZ_PROG_LEN 0x200

int main(int argc, const char *const *argv)
{
	seed_fast_rand(15);

	testing_initialize();

	static struct EXECUTOR_DATA_T executor_data;

	executor_init(&executor_data, IO_DATA_LEN, PROG_DATA_LEN);
	U8 * program_mem = EXECUTOR_PROGRAM_MEM(&executor_data);
	U8 * io_mem = EXECUTOR_IO_MEM(&executor_data);

	// Try "Hello world" a lot

	for (int i = 0; i < 10000; i++)
	{
		memset(io_mem, 0, IO_DATA_LEN);
		memcpy(program_mem, payload, sizeof(payload));
		int did_hang = executor_execute(&executor_data);
		testing_assert("executor_test_1_did_hang", !did_hang);
		testing_assert("executor_test_1_did_print", io_mem[0] == 'H');
	}
	// Fuzz and run hello world, look for code that forces the executor to bug out on successive runs

	for (int i = 0; i < 100000; i++)
	{
		// Fuzz
		memset(io_mem, 0, IO_DATA_LEN);
		memset(program_mem, 0x90, PROG_DATA_LEN);
		// Dummy problem
		struct Problem_T dummy_problem;
		dummy_problem.data_len = FUZZ_PROG_LEN;
		for (int j = 0; j < 1000; j++)
		{
			x86_basic_scramble(&dummy_problem, program_mem);
		}
		U8 rand_prog_data[FUZZ_PROG_LEN];
		memcpy(rand_prog_data, program_mem, FUZZ_PROG_LEN);
		U32 csum = qhashmurmur3_32(rand_prog_data, FUZZ_PROG_LEN);

		executor_execute(&executor_data);

		// Hello world
		memset(io_mem, 0, IO_DATA_LEN);
		memset(program_mem, 0x90, PROG_DATA_LEN);
		memcpy(program_mem, payload, sizeof(payload));
		int did_hang = executor_execute(&executor_data);
		testing_assert("executor_test_2_did_hang", !did_hang);
		if (!testing_assert("executor_test_2_did_print", io_mem[0] == 'H'))
		{
			printf("Checksum: %x\n", csum);
			printf("Program: \n");
			print_data_as_hex(rand_prog_data, FUZZ_PROG_LEN);
		}

	}

}
