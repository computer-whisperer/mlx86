#ifndef KVM_EXECUTOR_H
#define KVM_EXECUTOR_H

#include <ctime>
#include <cstddef>
#include "types.h"

struct kvm_executor_vcpu {
	int fd;
	struct kvm_run *kvm_run;
};

class KVMExecutor
{
    timer_t timer;
    int fd;
    struct kvm_executor_vcpu vcpus[1];
public:
    size_t program_memory_size;
    size_t program_memory_real_size;
    U8 * program_mem;
    size_t io_mem_size;
    size_t io_mem_real_size;
    U8 * io_mem;

    KVMExecutor(size_t io_memory_size, size_t program_memory_size);
    ~KVMExecutor();

    bool run();
};

#endif
