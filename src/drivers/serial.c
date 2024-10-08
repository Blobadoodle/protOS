#include <kernel/io.h>
#include <kernel/serial.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define PORT 0x3f8 // COM1

uint8_t serial_init() {
	outb(PORT + 1, 0x00);    // Disable all interrupts
	outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
	outb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
	outb(PORT + 1, 0x00);    //                  (hi byte)
	outb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
	outb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
	outb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
	outb(PORT + 4, 0x1E);    // Set in loopback mode, test the serial chip
	outb(PORT + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)

	// Check if serial is faulty (i.e: not same byte as sent)
	if(inb(PORT + 0) != 0xAE) {
		return 1;
	}

	// If serial is not faulty set it in normal operation mode
	// (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
	outb(PORT + 4, 0x0F);

	kprintfs("Serial initialised on port 0x%x\n", PORT);

	return 0;
}

bool serial_received() {
	return inb(PORT + 5) & 1;
}

char serial_read() {
	while (serial_received() == false);

	return inb(PORT);
}

bool serial_is_transmit_empty() {
	return inb(PORT + 5) & 0x20;
}

void serial_putchar(const char a) {
	while (serial_is_transmit_empty() == false);

	outb(PORT, a);
}

void serial_puts(const char *a) {
	for(size_t i = 0; i < strlen(a); i++)
		serial_putchar(a[i]);
}