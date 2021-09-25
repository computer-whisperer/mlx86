#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/kvm.h>
#include <fcntl.h>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <csignal>
#include <cstring>
#include <stdexcept>
#include "zydis.h"
#include "christian_utils.h"
#include "kvm_executor.h"
#include "types.h"

/* CR0 bits */
#define CR0_PE 1u


#define PROG_MEM_OFFSET 0x1000000

#define MIN_HALT_TRAP_LEN 0x1000
#define MEM_SEG_LEN 0x1000

thread_local char did_alarm = 0;
thread_local U8 did_interrupt = 0;

void catch_alarm(int sig)
{
	signal (sig, catch_alarm);
	did_alarm = 1;
}

void catch_stop_signal(int sig)
{
	signal (sig, catch_stop_signal);
	did_interrupt = 1;
}

thread_local U32 prev_exit_reason = 0;

KVMExecutor::KVMExecutor(size_t io_memory_size_in, size_t program_memory_size_in) :
io_mem_size(io_memory_size_in),
program_mem_size(program_memory_size_in)
{
    int api_ver;
    struct kvm_userspace_memory_region memreg{};

    sys_fd = open("/dev/kvm", O_RDWR);
    if (sys_fd < 0) {
        perror("open /dev/kvm");
        exit(1);
    }

    api_ver = ioctl(sys_fd, KVM_GET_API_VERSION, 0);
    if (api_ver < 0) {
        perror("KVM_GET_API_VERSION");
        exit(1);
    }

    if (api_ver != KVM_API_VERSION) {
        fprintf(stderr, "Got KVM api version %d, expected %d\n", api_ver, KVM_API_VERSION);
        exit(1);
    }

    fd = ioctl(sys_fd, KVM_CREATE_VM, 0);
    if (fd < 0) {
        // Blindly try it again...
        fd = ioctl(sys_fd, KVM_CREATE_VM, 0);
        if (fd < 0) {
            perror("KVM_CREATE_VM");
            exit(1);
        }
    }

    if (ioctl(fd, KVM_SET_TSS_ADDR, 0xfffbd000) < 0) {
        perror("KVM_SET_TSS_ADDR");
        exit(1);
    }

    U32 io_sector_num = ((io_mem_size)/MEM_SEG_LEN) + 1;
    io_mem_real_size = io_sector_num * MEM_SEG_LEN;

    // IO memory
    io_mem = static_cast<U8 *>(mmap(nullptr, io_mem_real_size, PROT_READ | PROT_WRITE,
                                        MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0));
    if (io_mem == MAP_FAILED) {
        perror("mmap io mem");
        exit(1);
    }

    memreg.slot = 0;
    memreg.flags = 0;
    memreg.guest_phys_addr = 0;
    memreg.memory_size = io_mem_real_size;
    memreg.userspace_addr = (unsigned long)io_mem;
    if (ioctl(fd, KVM_SET_USER_MEMORY_REGION, &memreg) < 0) {
        perror("KVM_SET_USER_MEMORY_REGION_A");
        exit(1);
    }

    U32 prog_sector_num = ((program_mem_size + MIN_HALT_TRAP_LEN)/MEM_SEG_LEN)+1;
    program_mem_real_size = prog_sector_num * MEM_SEG_LEN;

    // Program Memory
    program_mem = static_cast<U8 *>(mmap(nullptr, program_mem_real_size, PROT_READ | PROT_WRITE | PROT_EXEC,
                                             MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0));
    if (program_mem == MAP_FAILED) {
        perror("mmap program mem");
        exit(1);
    }

    memreg.slot = 1;
    memreg.flags = KVM_MEM_READONLY;
    memreg.guest_phys_addr = PROG_MEM_OFFSET;
    memreg.memory_size = program_mem_real_size;
    memreg.userspace_addr = (unsigned long)program_mem;
    if (ioctl(fd, KVM_SET_USER_MEMORY_REGION, &memreg) < 0) {
        perror("KVM_SET_USER_MEMORY_REGION_B");
        exit(1);
    }

    // Setup HALT trap
    memset(((U8*)program_mem) + program_mem_size, 0xf4, program_mem_real_size-program_mem_size);


    int vcpu_mmap_size;

    fd = ioctl(fd, KVM_CREATE_VCPU, 0);
    if (vcpus->fd < 0) {
        perror("KVM_CREATE_VCPU");

        exit(1);
    }

    vcpu_mmap_size = ioctl(sys_fd, KVM_GET_VCPU_MMAP_SIZE, 0);
    if (vcpu_mmap_size <= 0) {
        perror("KVM_GET_VCPU_MMAP_SIZE");
        exit(1);
    }

    vcpus->kvm_run = static_cast<kvm_run *>(mmap(nullptr, vcpu_mmap_size, PROT_READ | PROT_WRITE,
                                                MAP_SHARED, vcpus->fd, 0));
    if (vcpus->kvm_run == MAP_FAILED) {
        perror("mmap kvm_run");
        exit(1);
    }
}

KVMExecutor::~KVMExecutor() {
    int vcpu_mmap_size = ioctl(sys_fd, KVM_GET_VCPU_MMAP_SIZE, 0);
    if (vcpu_mmap_size <= 0) {
        perror("KVM_GET_VCPU_MMAP_SIZE");
        exit(1);
    }

    munmap(vcpus->kvm_run, vcpu_mmap_size);
    close(vcpus->fd);
    close(fd);
    close(sys_fd);
    munmap(program_mem, program_mem_real_size);
    munmap(io_mem, io_mem_real_size);
}

bool KVMExecutor::run() {

    struct kvm_sregs sregs{};
    struct kvm_regs regs{};
    struct kvm_fpu fpu{};

    // Set VCPU events

    // Briefly re-enter
    vcpus->kvm_run->immediate_exit =1;
    ioctl(vcpus->fd, KVM_RUN, 0);
    vcpus->kvm_run->immediate_exit =0;

    struct kvm_vcpu_events events;

    if (ioctl(vcpus->fd, KVM_GET_VCPU_EVENTS, &events) < 0)
    {
        throw std::runtime_error("KVM_GET_VCPU_EVENTS");
    }

    // Clear FPU
    if (ioctl(vcpus->fd, KVM_GET_FPU, &fpu) < 0)
    {
        throw std::runtime_error("KVM_GET_FPU");
    }
    memset(&fpu, 0, sizeof(fpu));
    if (ioctl(vcpus->fd, KVM_SET_FPU, &fpu) < 0)
    {
        throw std::runtime_error("KVM_SET_FPU");
    }

    if (ioctl(vcpus->fd, KVM_GET_SREGS, &sregs) < 0)
    {
        throw std::runtime_error("KVM_GET_SREGS");
    }

    if (ioctl(vcpus->fd, KVM_GET_REGS, &regs) < 0)
    {
        throw std::runtime_error("KVM_GET_REGS");
    }

    struct kvm_segment seg =
            {
            .base = 0,
            .limit = 0xffffffff,
            .selector = 1 << 3,
            .type = 11, /* Code: execute, read, accessed */
            .present = 1,
            .dpl = 0,
            .db = 1,
            .s = 1, /* Code/data */
            .l = 0,
            .g = 1, /* 4KB granularity */
            };

    sregs.cr0 |= CR0_PE; /* enter protected mode */

    sregs.cs = seg;

    seg.type = 3; /* Data: read/write, accessed */
    seg.selector = 2 << 3;
    sregs.ds = sregs.es = sregs.fs = sregs.gs = sregs.ss = seg;
    if (ioctl(vcpus->fd, KVM_SET_SREGS, &sregs) < 0)
    {
        throw std::runtime_error("KVM_SET_SREGS");
    }

    memset(&regs, 0, sizeof(regs));
    /* Clear all FLAGS bits, except bit 1 which is always set. */
    regs.rflags = 2;
    regs.rip = PROG_MEM_OFFSET;

    if (ioctl(vcpus->fd, KVM_SET_REGS, &regs) < 0)
    {
        throw std::runtime_error("KVM_SET_REGS");
    }

    memset(&events, 0, sizeof(events));
    if (ioctl(vcpus->fd, KVM_SET_VCPU_EVENTS, &events) < 0)
    {
        throw std::runtime_error("KVM_SET_VCPU_EVENTS");
    }

    did_alarm = 0;
    did_interrupt = 0;

    signal (SIGALRM, catch_alarm);
    signal (SIGUSR1, catch_stop_signal);


    prev_exit_reason = vcpus->kvm_run->exit_reason;

    if (ioctl(vcpus->fd, KVM_RUN, 0) < 0)
    {
        if (!did_alarm)
        {
            throw std::runtime_error("KVM_RUN");
        }
    }

    if (did_interrupt)
    {
        printf("Force killed executor!!!\n");
        if (ioctl(vcpus->fd, KVM_GET_REGS, &regs) < 0)
        {
            throw std::runtime_error("KVM_GET_REGS");
        }

        U32 offset = regs.rip - PROG_MEM_OFFSET;
        printf("Raw:\n");
        print_data_as_hex(program_mem + offset, 0x10);
        printf("Dissasembly:\n");
        zydis_print_instruction(program_mem + offset, program_mem_size - offset, regs.rip);
        printf("Code:\n");
        zydis_print_dissasembly(program_mem, program_mem_size);
        FILE * fp = fopen("debug_data", "wb");
        fwrite(program_mem, sizeof(U8), program_mem_size, fp);
        fclose(fp);

    }

    // Check that it did something

    if (ioctl(vcpus->fd, KVM_GET_REGS, &regs) < 0)
    {
        throw std::runtime_error("KVM_GET_REGS");
    }

    if (ioctl(vcpus->fd, KVM_GET_VCPU_EVENTS, &events) < 0)
    {
        throw std::runtime_error("KVM_GET_VCPU_EVENTS");
    }



    if (vcpus->kvm_run->exit_reason == KVM_EXIT_HLT)
    {
        // Expected return route

    }

    else if (vcpus->kvm_run->exit_reason == KVM_EXIT_MMIO)
    {
        // Looks like we have to clean up after this event
        U32 prev_addr = vcpus->kvm_run->mmio.phys_addr;

        // Briefly re-enter
        vcpus->kvm_run->immediate_exit =1;
        S32 r2 = 0;
        U32 i;
        for (i = 0; i < 10; i++)
        {
            r2 = ioctl(vcpus->fd, KVM_RUN, 0);
            if (r2 == -1)
            {
                break;
            }
        }
        //testing_assert("kvm_executor_mmio_clear", i < 10);
        vcpus->kvm_run->immediate_exit =0;

        if (ioctl(vcpus->fd, KVM_GET_REGS, &regs) < 0)
        {
            throw std::runtime_error("KVM_GET_REGS");
        }

        if (ioctl(vcpus->fd, KVM_GET_VCPU_EVENTS, &events) < 0)
        {
            throw std::runtime_error("KVM_GET_VCPU_EVENTS");
        }
    }

    else if (vcpus->kvm_run->exit_reason == KVM_EXIT_INTR)
    {
        // Watchdog alarm fired

    }

    else if (vcpus->kvm_run->exit_reason == KVM_EXIT_SHUTDOWN)
    {
        /*
		perror("KVM in shutdown!");

	    if (ioctl(executor_data->vcpus->fd, KVM_GET_VCPU_EVENTS, &events) < 0)
	    {
			perror("KVM_GET_REGS");
			exit(1);
		}

		exit(1);
		*/
    }

    else if (vcpus->kvm_run->exit_reason == KVM_EXIT_IO)
    {
        vcpus->kvm_run->immediate_exit =1;
        ioctl(vcpus->fd, KVM_RUN, 0);
        vcpus->kvm_run->immediate_exit =0;
    }

    else if (vcpus->kvm_run->exit_reason == KVM_EXIT_INTERNAL_ERROR)
    {
        // Nothing to do
    }


    else if (vcpus->kvm_run->exit_reason == KVM_EXIT_FAIL_ENTRY)
    {
        // Unknown

    }

    else
    {
        printf("Strange VM exit: %d\n", vcpus->kvm_run->exit_reason);
    }


    return did_alarm;
}
