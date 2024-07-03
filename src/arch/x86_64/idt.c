#include <kernel/idt.h>
#include <kernel/io.h>
#include <kernel/pic.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

typedef struct {
	uint16_t size;
	uint64_t offset;
} __attribute__((packed)) idtr_t;

typedef struct {
	uint16_t offset_low; // Lower 16 bits of ISR address
	uint16_t selector; // Segment selector, point to valid code segment in GDT
	uint8_t  ist; // Offset to interrupt stack table in task state segment
	uint8_t  attributes; // See IDT page on osdev
	uint16_t offset_mid; // Mid 16 bits of ISR address
	uint32_t offset_high; // Upper 32 bits of ISR address
	uint32_t reserved; // Reserved
} __attribute__((packed)) idt_entry_t;

__attribute__ ((aligned(0x10)))
static idt_entry_t idt[256];

static idtr_t idtr;

extern void* isr_stub_table[];
extern void* irq_stub_table[];

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t *descriptor = &idt[vector];

    descriptor->offset_low     = (uint64_t)isr & 0xFFFF;
    descriptor->selector       = 0x08; // GDT Kernel code segment selector
    descriptor->ist            = 0;
    descriptor->attributes     = flags;
    descriptor->offset_mid     = ((uint64_t)isr >> 16) & 0xFFFF;
    descriptor->offset_high    = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    descriptor->reserved       = 0;
}

// array of functions to be called via irq_handler, can be registed and unregistered with idt_register_handler, idt_unregister_handler
void (*irqs[16])() = {NULL};

void irq_handler(uint64_t irq) {
	void (*irq_registered_handler)() = irqs[irq];

	if(irq_registered_handler == NULL) {
		panic("Unregistered IRQ.");
	}

	irq_registered_handler();

	pic_send_eoi((uint8_t)irq);
}

void idt_register_handler(uint8_t irq, void(*handler)()) {
	irqs[irq] = handler;
}

void idt_unregister_handler(uint8_t irq) {
	irqs[irq] = NULL;
}

void idt_init() {
	idtr.size = (uint16_t)(sizeof(idt) - 1);
	idtr.offset = (uint64_t)&idt[0];

	for(uint8_t vector = 0; vector < 48; vector++) {
		if(vector >= 32) { // IRQ
			idt_set_descriptor(vector, irq_stub_table[vector - 32], 0x8E);
		} else {
			idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
		}
	}

	__asm__ volatile ("lidt %0" : : "m"(idtr)); // tell cpu where IDT is

	sti();

	kprintfs("IDT Installed.\n");
}