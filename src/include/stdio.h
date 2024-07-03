#ifndef _STDIO_H
#define _STDIO_H

#include <stdint.h>

int putchar(int a);
int puts(const char *str);
uint64_t kprintf(const char *format, ...);
uint64_t kprintfs(const char *format, ...);

#endif