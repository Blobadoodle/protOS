#include <stdint.h>
#include <kernel/io.h>

void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

void io_wait() {
    outb(0x80, 0);
}

__attribute__((noreturn))
void halt() {
    for (;;) {
        __asm__ volatile ("cli; hlt");
    }
}

void cli() {
    __asm__ volatile ("cli");
}

void sti() {
    __asm__ volatile ("sti");
}
