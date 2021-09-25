#ifndef KVM_EXECUTOR_H
#define KVM_EXECUTOR_H

#include <ctime>
#include <cstddef>
#include <thread>
#include "types.h"

struct kvm_executor_vcpu {
	int fd;
	struct kvm_run *kvm_run;
};

class KVMExecutor
{
    std::thread watchdog_thread;
    timer_t timer;
    int fd;
    int sys_fd;
    struct kvm_executor_vcpu vcpus[1];
public:
    size_t program_memory_size;
    size_t program_memory_real_size;
    U8 * program_memory;
    size_t io_mem_size;
    size_t io_mem_real_size;
    U8 * io_memory;
    long execution_idx = 0;
    bool is_executing = false;
    bool should_watchdog_stop = false;

    void StartWatchdogThread();

    KVMExecutor(size_t io_memory_size, size_t program_memory_size);
    ~KVMExecutor();

    bool run();
};

#endif
