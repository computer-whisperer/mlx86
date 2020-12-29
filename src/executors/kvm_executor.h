#ifndef KVM_EXECUTOR_H
#define KVM_EXECUTOR_H

#include <stddef.h>
#include "types.h"

struct kvm_executor_vcpu {
	int fd;
	struct kvm_run *kvm_run;
};

struct EXECUTOR_DATA_T
{
	int sys_fd;
	int fd;
	char *mem;
	struct kvm_executor_vcpu vcpus[1];
};

void executor_init(struct EXECUTOR_DATA_T * executor_data, size_t memory_size);

int executor_execute(struct EXECUTOR_DATA_T * executor_data, U64 ip);

#define EXECUTOR_PROCESS_MEM(executor_data) (executor_data)->mem

#endif
