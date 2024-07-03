#include <kernel/pic.h>
#include <kernel/io.h>
#include <stdint.h>
#include <stdio.h>

#define PIC1		0x20		// IO base address for master PIC
#define PIC2		0xA0		// IO base address for slave PIC
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

#define ICW1_ICW4	0x01		// Indicates that ICW4 will be present 
#define ICW1_SINGLE	0x02		// Single (cascade) mode 
#define ICW1_INTERVAL4	0x04	// Call address interval 4 (8) 
#define ICW1_LEVEL	0x08		// Level triggered (edge) mode 
#define ICW1_INIT	0x10		// Initialization - required! 

#define ICW4_8086	0x01		// 8086/88 (MCS-80/85) mode 
#define ICW4_AUTO	0x02		// Auto (normal) EOI 
#define ICW4_BUF_SLAVE	0x08	// Buffered mode/slave 
#define ICW4_BUF_MASTER	0x0C	// Buffered mode/master 
#define ICW4_SFNM	0x10		// Special fully nested (not)

#define PIC_EOI		0x20

#define PIC1_OFFSET 0x20
#define PIC2_OFFSET 0x28

void pic_disable() {
	outb(PIC1_DATA, 0xff);
	outb(PIC2_DATA, 0xff);
}

void pic_init() {
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4); // starts init sequence in cascade mode
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, PIC1_OFFSET); // ICW2: master pic offset
	io_wait();
	outb(PIC2_DATA, PIC2_OFFSET); // ICW2: slave pic offset
	io_wait();
	outb(PIC1_DATA, 4); // ICW3: tell master pic there is a slave pic at irq 2
	io_wait();
	outb(PIC2_DATA, 2); // ICW3: tell slave pic it's ascade identity (2)
	io_wait();

	outb(PIC1_DATA, ICW4_8086); // have PICs use 8086 mode (not 8088)
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();

	pic_disable(); // mask every interrupt

	kprintfs("PIC remapped to offsets 0x%x & 0x%x.\n", PIC1_OFFSET, PIC2_OFFSET);
}

void pic_clear_mask(uint8_t irq) {
	uint16_t port;
	uint8_t value;

	if(irq < 8) {
		port = PIC1_DATA;
	} else {
		port = PIC2_DATA;
		irq -= 8;
	}

	value = inb(port) & ~(1 << irq);
	outb(port, value);
}

void pic_set_mask(uint8_t irq) {
	uint16_t port;
	uint8_t value;

	if(irq < 8) {
		port = PIC1_DATA;
	} else {
		port = PIC2_DATA;
		irq -= 8;
	}

	value = inb(port) | (1 << irq);
	outb(port, value);
}

void pic_send_eoi(uint8_t irq) {
	if(irq >= 8)
		outb(PIC2_COMMAND, PIC_EOI);

	outb(PIC1_COMMAND, PIC_EOI);
}