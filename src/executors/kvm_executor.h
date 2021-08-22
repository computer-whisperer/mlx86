#ifndef KVM_EXECUTOR_H
#define KVM_EXECUTOR_H

#include <time.h>
#include <stddef.h>
#include "types.h"

struct kvm_executor_vcpu {
	int fd;
	struct kvm_run *kvm_run;
};

struct EXECUTOR_DATA_T
{
	timer_t timer;
	int sys_fd;
	int fd;
	size_t program_mem_size;
	size_t program_mem_real_size;
	void * program_mem;
	size_t io_mem_size;
	size_t io_mem_real_size;
	void * io_mem;
	struct kvm_executor_vcpu vcpus[1];
};

void executor_init(struct EXECUTOR_DATA_T * executor_data_out, size_t io_memory_size, size_t program_memory_size);

void executor_deinit(struct EXECUTOR_DATA_T * executor_data);

int executor_execute(struct EXECUTOR_DATA_T * executor_data);

#define EXECUTOR_PROGRAM_MEM(executor_data) (executor_data)->program_mem
#define EXECUTOR_IO_MEM(executor_data) (executor_data)->io_mem

#endif
