#ifndef _KERNEL_IDT_H
#define _KERNEL_IDT_H

#include <stdint.h>

void idt_init();
void idt_register_handler(uint8_t irq, void(*handler)());
void idt_unregister_handler(uint8_t irq);

#endif