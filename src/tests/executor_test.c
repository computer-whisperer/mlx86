#include "types.h"
#include "executor.h"
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

static int executor_initialized = 0;
static struct EXECUTOR_DATA_T executor_data;
static struct ml86_process_memory * proc_mem;

int main(int argc, const char *const *argv)
{
	executor_init(&executor_data, sizeof(struct ml86_process_memory));

	for (int i = 0; i < 1000000; i++)
	{
		proc_mem = (struct ml86_process_memory *)EXECUTOR_PROCESS_MEM(&executor_data);
		memset(proc_mem->io_data, 0, IO_DATA_LEN);
		memset(proc_mem->program_data, 0x90, PROG_DATA_LEN);
		memset(proc_mem->end_sled, 0xf4, END_SLED_LEN);
		memcpy(proc_mem->program_data, payload, sizeof(payload));
		int did_hang = executor_execute(&executor_data, PROG_DATA_LEN);

		if (did_hang)
		{
			printf("Timeout!\n");
		}
		else if (proc_mem->io_data[0] != 'H')
		{
			printf("It stopped working!\n");
		}
	}

	printf("Executor says: %s\n", proc_mem->io_data);
}
