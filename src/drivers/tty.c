#define SSFN_CONSOLEBITMAP_TRUECOLOR
#define SSFN_CONSOLEBITMAP_CONTROL

#include <kernel/tty.h>
#include <kernel/io.h>
#include <stddef.h>
#include <stdint.h>
#include <ssfn.h>
#include <limine.h>
#include <stdio.h>

// Get limine framebuffer
__attribute__((used, section(".requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

// Get kernel module which stores the font
__attribute__((used, section(".requests")))
static volatile struct limine_module_request module_request = {
	.id = LIMINE_MODULE_REQUEST,
	.revision = 1
};

void tty_init() {
	if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1) {
		kprintfs("[FATAL] Limine provided no framebuffer.\n");
        halt();
    }

	if (module_request.response == NULL || module_request.response->module_count < 1) {
		kprintfs("[FATAL] Limine font module.\n");
		halt();
	}

	struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

	// TODO: get by filename, not just first module
	struct limine_file font_file = *module_request.response->modules[0];

	ssfn_src = font_file.address;

	ssfn_dst.ptr = framebuffer->address;
	ssfn_dst.w = framebuffer->width;
	ssfn_dst.h = framebuffer->height;
	ssfn_dst.p = framebuffer->pitch;
	ssfn_dst.x = ssfn_dst.y = 0;
	ssfn_dst.fg = 0xFFFFFF;
	ssfn_dst.bg = 0x000000;
}

void tty_putchar(char a) {
	ssfn_putc(a);
}

void tty_set_fg(uint32_t fg) {
	ssfn_dst.fg = fg;
}

void tty_set_bg(uint32_t bg) {
	ssfn_dst.bg = bg;
}

void tty_set_x(int x) {
	ssfn_dst.x = x;
}

void tty_set_y(int y) {
	ssfn_dst.y = y;
}

int tty_get_x() {
	return ssfn_dst.x;
}

int tty_get_y() {
	return ssfn_dst.y;
}

void tty_clear() {
	struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    volatile uint32_t *fb_ptr = framebuffer->address;

    for (size_t y = 0; y < framebuffer->height; y++) {
    	for (size_t x = 0; x < framebuffer->width; x++) {
        	fb_ptr[y * (framebuffer->pitch / 4) + x] = ssfn_dst.bg;
		}
    }

	ssfn_dst.x = ssfn_dst.y = 0;
}