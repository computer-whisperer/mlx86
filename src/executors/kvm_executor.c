#include <sys/mman.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/kvm.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "kvm_executor.h"
#include "types.h"
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include "signal.h"

/* CR0 bits */
#define CR0_PE 1u
#define CR0_MP (1U << 1)
#define CR0_EM (1U << 2)
#define CR0_TS (1U << 3)
#define CR0_ET (1U << 4)
#define CR0_NE (1U << 5)
#define CR0_WP (1U << 16)
#define CR0_AM (1U << 18)
#define CR0_NW (1U << 29)
#define CR0_CD (1U << 30)
#define CR0_PG (1U << 31)

/* CR4 bits */
#define CR4_VME 1
#define CR4_PVI (1U << 1)
#define CR4_TSD (1U << 2)
#define CR4_DE (1U << 3)
#define CR4_PSE (1U << 4)
#define CR4_PAE (1U << 5)
#define CR4_MCE (1U << 6)
#define CR4_PGE (1U << 7)
#define CR4_PCE (1U << 8)
#define CR4_OSFXSR (1U << 8)
#define CR4_OSXMMEXCPT (1U << 10)
#define CR4_UMIP (1U << 11)
#define CR4_VMXE (1U << 13)
#define CR4_SMXE (1U << 14)
#define CR4_FSGSBASE (1U << 16)
#define CR4_PCIDE (1U << 17)
#define CR4_OSXSAVE (1U << 18)
#define CR4_SMEP (1U << 20)
#define CR4_SMAP (1U << 21)

#define EFER_SCE 1
#define EFER_LME (1U << 8)
#define EFER_LMA (1U << 10)
#define EFER_NXE (1U << 11)

/* 32-bit page directory entry bits */
#define PDE32_PRESENT 1
#define PDE32_RW (1U << 1)
#define PDE32_USER (1U << 2)
#define PDE32_PS (1U << 7)

/* 64-bit page * entry bits */
#define PDE64_PRESENT 1
#define PDE64_RW (1U << 1)
#define PDE64_USER (1U << 2)
#define PDE64_ACCESSED (1U << 5)
#define PDE64_DIRTY (1U << 6)
#define PDE64_PS (1U << 7)
#define PDE64_G (1U << 8)

void vm_init(struct EXECUTOR_DATA_T *vm, size_t mem_size)
{
	int api_ver;
	struct kvm_userspace_memory_region memreg;

	vm->sys_fd = open("/dev/kvm", O_RDWR);
	if (vm->sys_fd < 0) {
		perror("open /dev/kvm");
		exit(1);
	}

	api_ver = ioctl(vm->sys_fd, KVM_GET_API_VERSION, 0);
	if (api_ver < 0) {
		perror("KVM_GET_API_VERSION");
		exit(1);
	}

	if (api_ver != KVM_API_VERSION) {
		fprintf(stderr, "Got KVM api version %d, expected %d\n", api_ver, KVM_API_VERSION);
		exit(1);
	}

	vm->fd = ioctl(vm->sys_fd, KVM_CREATE_VM, 0);
	if (vm->fd < 0) {
    // Blindly try it again...
    vm->fd = ioctl(vm->sys_fd, KVM_CREATE_VM, 0);
    if (vm->fd < 0) {
      perror("KVM_CREATE_VM");
      exit(1);
    }
	}

    if (ioctl(vm->fd, KVM_SET_TSS_ADDR, 0xfffbd000) < 0) {
        perror("KVM_SET_TSS_ADDR");
		exit(1);
	}

	vm->mem = mmap(NULL, mem_size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
	if (vm->mem == MAP_FAILED) {
		perror("mmap mem");
		exit(1);
	}

	memreg.slot = 0;
	memreg.flags = 0;
	memreg.guest_phys_addr = 0;
	memreg.memory_size = mem_size;
	memreg.userspace_addr = (unsigned long)vm->mem;
        if (ioctl(vm->fd, KVM_SET_USER_MEMORY_REGION, &memreg) < 0) {
		perror("KVM_SET_USER_MEMORY_REGION");
                exit(1);
	}
}

void vm_deinit(struct EXECUTOR_DATA_T *vm, size_t mem_size)
{
  munmap(vm->mem, mem_size);
  close(vm->fd);
  close(vm->sys_fd);
}

void vcpu_init(struct EXECUTOR_DATA_T *vm, struct kvm_executor_vcpu *vcpu)
{
	int vcpu_mmap_size;

	vcpu->fd = ioctl(vm->fd, KVM_CREATE_VCPU, 0);
        if (vcpu->fd < 0) {
		perror("KVM_CREATE_VCPU");

                exit(1);
	}

	vcpu_mmap_size = ioctl(vm->sys_fd, KVM_GET_VCPU_MMAP_SIZE, 0);
        if (vcpu_mmap_size <= 0) {
		perror("KVM_GET_VCPU_MMAP_SIZE");
                exit(1);
	}

	vcpu->kvm_run = mmap(NULL, vcpu_mmap_size, PROT_READ | PROT_WRITE,
			     MAP_SHARED, vcpu->fd, 0);
	if (vcpu->kvm_run == MAP_FAILED) {
		perror("mmap kvm_run");
		exit(1);
	}
}

void vcpu_deinit(struct EXECUTOR_DATA_T *vm, struct kvm_executor_vcpu *vcpu)
{
  int vcpu_mmap_size = ioctl(vm->sys_fd, KVM_GET_VCPU_MMAP_SIZE, 0);
  if (vcpu_mmap_size <= 0) {
		perror("KVM_GET_VCPU_MMAP_SIZE");
                exit(1);
	}

  munmap(vcpu->kvm_run, vcpu_mmap_size);
  
  close(vm->sys_fd);
}

void executor_init(struct EXECUTOR_DATA_T * executor_data, size_t memory_size)
{
	vm_init(executor_data, memory_size);
	vcpu_init(executor_data, executor_data->vcpus);
}

void executor_deinit(struct EXECUTOR_DATA_T * executor_data, size_t memory_size)
{
  vcpu_deinit(executor_data, executor_data->vcpus);
  vm_deinit(executor_data, memory_size);
}

char did_alarm = 0;

void catch_alarm(int sig)
{
  signal (sig, catch_alarm);
  did_alarm = 1;
}

U32 prev_exit_reason = 0;

int executor_execute(struct EXECUTOR_DATA_T * executor_data, U64 ip)
{
	struct kvm_sregs sregs;
	struct kvm_regs regs;
	struct kvm_fpu fpu;

	// Set VCPU events

	// Briefly re-enter
	executor_data->vcpus->kvm_run->immediate_exit =1;
	ioctl(executor_data->vcpus->fd, KVM_RUN, 0);
	executor_data->vcpus->kvm_run->immediate_exit =0;

	struct kvm_vcpu_events events;

    if (ioctl(executor_data->vcpus->fd, KVM_GET_VCPU_EVENTS, &events) < 0)
    {
		perror("KVM_GET_VCPU_EVENTS");
		exit(1);
	}

	// Clear FPU
    if (ioctl(executor_data->vcpus->fd, KVM_GET_FPU, &fpu) < 0)
    {
		perror("KVM_GET_FPU");
		exit(1);
	}
    memset(&fpu, 0, sizeof(fpu));
    if (ioctl(executor_data->vcpus->fd, KVM_SET_FPU, &fpu) < 0)
    {
		perror("KVM_SET_FPU");
		exit(1);
	}

    if (ioctl(executor_data->vcpus->fd, KVM_GET_SREGS, &sregs) < 0)
    {
		perror("KVM_GET_SREGS");
		exit(1);
	}

    if (ioctl(executor_data->vcpus->fd, KVM_GET_REGS, &regs) < 0)
    {
		perror("KVM_GET_REGS");
		exit(1);
	}

	struct kvm_segment seg =
	{
		.base = 0,
		.limit = 0xffffffff,
		.selector = 1 << 3,
		.present = 1,
		.type = 11, /* Code: execute, read, accessed */
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
    if (ioctl(executor_data->vcpus->fd, KVM_SET_SREGS, &sregs) < 0)
    {
		perror("KVM_SET_SREGS");
		exit(1);
	}

	memset(&regs, 0, sizeof(regs));
	/* Clear all FLAGS bits, except bit 1 which is always set. */
	regs.rflags = 2;
	regs.rip = ip;

	if (ioctl(executor_data->vcpus->fd, KVM_SET_REGS, &regs) < 0)
	{
		perror("KVM_SET_REGS");
		exit(1);
	}

	memset(&events, 0, sizeof(events));
    if (ioctl(executor_data->vcpus->fd, KVM_SET_VCPU_EVENTS, &events) < 0)
    {
		perror("KVM_SET_VCPU_EVENTS");
		exit(1);
	}

	signal (SIGALRM, catch_alarm);
	did_alarm = 0;

	prev_exit_reason = executor_data->vcpus->kvm_run->exit_reason;

	struct itimerval timer;
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 1000;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 0;
	int rc = setitimer(ITIMER_REAL, &timer, NULL);

	if (ioctl(executor_data->vcpus->fd, KVM_RUN, 0) < 0)
	{
		if (!did_alarm)
		{
			perror("KVM_RUN");
			exit(1);
		}
	}

	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 0;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 0;
	if (setitimer(ITIMER_REAL, &timer, NULL))
	{
		printf("here");
	}

	// Check that it did something

    if (ioctl(executor_data->vcpus->fd, KVM_GET_REGS, &regs) < 0)
    {
		perror("KVM_GET_REGS");
		exit(1);
	}

    if (ioctl(executor_data->vcpus->fd, KVM_GET_VCPU_EVENTS, &events) < 0)
    {
		perror("KVM_GET_VCPU_EVENTS");
		exit(1);
	}



	if (executor_data->vcpus->kvm_run->exit_reason == KVM_EXIT_HLT)
	{
		// Expected return route

	}

	else if (executor_data->vcpus->kvm_run->exit_reason == KVM_EXIT_MMIO)
	{
		// Looks like we have to clean up after this event
			U32 prev_addr = executor_data->vcpus->kvm_run->mmio.phys_addr;

			// Briefly re-enter
			executor_data->vcpus->kvm_run->immediate_exit =1;
			S32 r2 = 0;
			U32 i;
			for (i = 0; i < 10; i++)
			{
				r2 = ioctl(executor_data->vcpus->fd, KVM_RUN, 0);
				if (r2 == -1)
				{
					break;
				}
			}
			testing_assert("kvm_executor_mmio_clear", i < 10);
			executor_data->vcpus->kvm_run->immediate_exit =0;

			if (ioctl(executor_data->vcpus->fd, KVM_GET_REGS, &regs) < 0)
			{
				perror("KVM_GET_REGS");
				exit(1);
			}

			if (ioctl(executor_data->vcpus->fd, KVM_GET_VCPU_EVENTS, &events) < 0)
			{
				perror("KVM_GET_VCPU_EVENTS");
				exit(1);
			}
	}

	else if (executor_data->vcpus->kvm_run->exit_reason == KVM_EXIT_INTR)
	{
		// Watchdog alarm fired

	}

	else if (executor_data->vcpus->kvm_run->exit_reason == KVM_EXIT_SHUTDOWN)
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

	else if (executor_data->vcpus->kvm_run->exit_reason == KVM_EXIT_IO)
	{
		executor_data->vcpus->kvm_run->immediate_exit =1;
		ioctl(executor_data->vcpus->fd, KVM_RUN, 0);
		executor_data->vcpus->kvm_run->immediate_exit =0;
	}

	else if (executor_data->vcpus->kvm_run->exit_reason == KVM_EXIT_INTERNAL_ERROR)
	{
		// Nothing to do
	}


	else if (executor_data->vcpus->kvm_run->exit_reason == KVM_EXIT_FAIL_ENTRY)
	{
		// Unknown

	}

	else
	{
		printf("Strange VM exit: %d\n", executor_data->vcpus->kvm_run->exit_reason);
	}


	return did_alarm;
}
