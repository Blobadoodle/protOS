#ifndef _KERNEL_SERIAL_H
#define _KERNEL_SERIAL_H

#include <stdint.h>

uint8_t serial_init();
char serial_read();
void serial_putchar(const char a);
void serial_puts(const char *a);

#endif