#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stdint.h>

void tty_init();
void tty_putchar(char a);
void tty_set_fg(uint32_t fg);
void tty_set_bg(uint32_t bg);
void tty_set_x(int x);
void tty_set_y(int y);
int tty_get_x();
int tty_get_y();
void tty_clear();

#endif