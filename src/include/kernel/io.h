#ifndef _KERNEL_IO_H
#define _KERNEL_IO_H

#include <stdint.h>

void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
void io_wait();
void cli();
void sti();
__attribute__((noreturn))
void halt();
__attribute__((noreturn))
void panic(const char *str);

#endif