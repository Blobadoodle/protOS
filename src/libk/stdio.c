#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <kernel/tty.h>
#include <kernel/serial.h>

int putchar(int a) {
	tty_putchar((char)a);
	return a;
}

int puts(const char *str) {
	size_t size = strlen(str);
	for (size_t i = 0; i < size; i++)
		putchar(str[i]);

	return size;
}

uint64_t print_uint(uint64_t x, uint8_t base, void(*putchar)(const char)) {
    char out[sizeof(uint64_t) * 2 + 3];
    uint64_t i, j, d;

    if(base != 10 && base != 16) return 0;

    if(x == 0) {
        putchar('0');
        return 1;
    }

    for(i = 0; x; x /= base, i++) {
        d = x % base;
        if(d >= 10) {
            out[i] = 'a' + d - 10;
        } else {
            out[i] = '0' + d;
        }
    }

    for(j = i; j > 0; j--) {
        putchar(out[j - 1]);
    }

    return i;
}

void print(const char *str, uint64_t size, void(*putchar)(const char)) {
    for(uint64_t i = 0; i < size; i++) putchar(str[i]);
}

uint64_t kvprintf(const char *format, va_list parameters, void(*putchar)(const char)) {
    uint64_t written = 0;

    while(*format != '\0') {
        if(format[0] != '%' || format[1] == '%') {
            if(format[0] == '%') format++;

            size_t amount = 1;
            while(format[amount] && format[amount] != '%') amount++;

            print(format, amount, putchar);

            format += amount;
            written += amount;
            continue;
        }

        const char* format_begun_at = format++;

        switch(*format) {
            case 'c': {
                format++;
                char c = (char) va_arg(parameters, int); // char is int kinda
                putchar(c);
                written++;
                break;
            } case 's': {
                format++;
                const char* str = va_arg(parameters, const char*);
                size_t len = strlen(str);
                print(str, len, putchar);
                written += len;
                break;
            } case 'd': {
                format++;
                int x = va_arg(parameters, int);

                if(x < 0) {
                    putchar('-');
                    written++;
                    x = -x;
                }

                written += print_uint(x, 10, putchar);
                break;
            } case 'x': {
                format++;
                uint64_t x = va_arg(parameters, uint64_t);
                written += print_uint(x, 16, putchar);
                break;
            } default: {
                format = format_begun_at;
                size_t len = strlen(format);
                print(format, len, tty_putchar);
                written += len;
                format += len;
            }

        }
    }

    return written;
}

uint64_t kprintf(const char *format, ...) {
    va_list args;
    uint64_t i;

    va_start(args, format);
    i = kvprintf(format, args, tty_putchar);
    va_end(args);

    return i;
}

uint64_t kprintfs(const char *format, ...) { // Serial printf
    va_list args;
    uint64_t i;

    va_start(args, format);
    i = kvprintf(format, args, serial_putchar);
    va_end(args);

    return i;
}