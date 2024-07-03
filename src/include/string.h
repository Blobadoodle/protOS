#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>
#include <stdint.h>

void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
void *memmove(void *dest, const void *src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
int strcmp(const char *x, const char *y);
int strncmp(const char *x, const char *y, size_t n);
size_t strlen(const char* str);
const char * strcpy(char dest[], const char source[]);
char *strcat(char *dest, const char *source);

#endif