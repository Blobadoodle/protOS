#ifndef _KERNEL_PIC_H
#define _KERNEL_PIC_H

#include <stdint.h>

void pic_init(); // remap pic to 32 -> 47
void pic_send_eoi(uint8_t irq); // end of interrupt
void pic_disable(); // masks every interrupt
void pic_set_mask(uint8_t irq); // ignore IRQ
void pic_clear_mask(uint8_t irq); // stop ignoring IRQ

#endif