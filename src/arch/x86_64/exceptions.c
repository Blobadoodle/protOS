#include <stdio.h>
#include <kernel/io.h>
#include <kernel/tty.h>

typedef struct {
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;

	uint64_t gs;
	uint64_t fs;
	uint64_t ss;
	uint64_t es;
	uint64_t ds;
	uint64_t cs;

	uint64_t rbp;
	uint64_t rdi;
	uint64_t rsi;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;

	uint64_t vector;
	uint64_t err;
} __attribute__((packed)) interrupt_context_t;


char *exceptions[] = {
	"Division Error",
	"Debug",
	"Non-Maskable Interrupt",
	"Breakpoint",
	"Overflow",
	"Bound Range Exceeded",
	"Invalid Opcode",
	"Device Not Available",
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Invalid TSS",
	"Segment Not Present",
	"Stack-Segment Fault",
	"General Protection Fault",
	"Page Fault",
	0, // reserved
	"x87 Floating-Point Exception",
	"Alignment Check",
	"Machine check",
	"SIMD Floating-Point Exception",
	"Virtualisation Exception",
	"Control Protection Exception",
	0, 0, 0, 0, 0, 0, // reserved
	"Hypervisor Injection Exception",
	"VMM Communication Exception",
	"Security Exception",
	0 // reserved
};

__attribute__((noreturn))
void exception_handler(interrupt_context_t *context) {
	tty_set_bg(0x8B0000);
    tty_clear();

	kprintf("PANIC! An unhandled exception has occured.\n");
	kprintf("[0x%x] %s\nError Code: %d (0x%x)\n\n", context->vector, exceptions[context->vector], context->err, context->err);

	kprintf("RAX = 0x%x\n", context->rax);
	kprintf("RBX = 0x%x\n", context->rbx);
	kprintf("RCX = 0x%x\n", context->rcx);
	kprintf("RDX = 0x%x\n", context->rdx);
	kprintf("RSI = 0x%x\n", context->rsi);
	kprintf("RDI = 0x%x\n", context->rdi);
	kprintf("RBP = 0x%x\n\n", context->rbp);

	kprintf("CS  = 0x%x\n", context->cs);
	kprintf("DS  = 0x%x\n", context->ds);
	kprintf("ES  = 0x%x\n", context->es);
	kprintf("SS  = 0x%x\n", context->ss);
	kprintf("FS  = 0x%x\n", context->fs);
	kprintf("GS  = 0x%x\n\n", context->gs);

	kprintf("R8  = 0x%x\n", context->r8);
	kprintf("R9  = 0x%x\n", context->r9);
	kprintf("R10 = 0x%x\n", context->r10);
	kprintf("R11 = 0x%x\n", context->r11);
	kprintf("R12 = 0x%x\n", context->r12);
	kprintf("R13 = 0x%x\n", context->r13);
	kprintf("R14 = 0x%x\n", context->r14);
	kprintf("R15 = 0x%x\n", context->r15);

	halt();
}

__attribute__((noreturn))
void panic(const char *str) {
	tty_set_bg(0x8B0000);
    tty_clear();

	kprintf("PANIC! An unhandled exception has occured.\n");
	puts(str);

	halt();
}