#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>
#include <string.h>
#include <kernel/serial.h>
#include <kernel/tty.h>
#include <kernel/io.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/pic.h>
#include <kernel/ps2.h>
#include <stdio.h>

__attribute__((used, section(".requests")))
static volatile LIMINE_BASE_REVISION(2);

__attribute__((used, section(".requests_start_marker")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".requests_end_marker")))
static volatile LIMINE_REQUESTS_END_MARKER;

void _start(void) {
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        halt();
    }

    serial_init();
    tty_init();
    gdt_init();
    pic_init();
    idt_init();
    ps2_init();

    sti();

    // __asm__  ("div %0" :: "r"(0));

    puts("protOS\n>\n");

    // halt();
    for(;;) {
        __asm__ volatile ("hlt");
    }
}