/*
 * Copyright (C) 2023 Ángel Pérez <ap@anpep.co>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>

extern int early_putchar(int);

int putchar(int c)
{
    // TODO: Invoke the kernel console drivers.
    return early_putchar(c);
}

int puts(const char* s)
{
    if (s == NULL)
        return puts("(null)");
    while (*s != '\0')
        putchar(*s++);
    return 0;
}

int printf(const char* restrict fmt, ...)
{
    int rc;
    va_list v;
    va_start(v, fmt);
    rc = vprintf(fmt, v);
    va_end(v);
    return rc;
}

int snprintf(char* restrict dest, size_t n, const char* restrict fmt, ...)
{
    return -1;
}

int sprintf(char* restrict dest, const char* restrict fmt, ...) { return -1; }

static unsigned int _str2int(const char* restrict s, int* val)
{
    unsigned int v = 0;
    size_t i;
    for (i = 0; *s >= '0' && *s <= '9'; i++)
        v = 10 * v + *s++ - '0';
    *val = v;
    return i - 1;
}

static void _int2str(char* dest, char spec, unsigned int value, size_t dest_len)
{
    if (dest_len < 2)
        return;

    char* head = dest;
    unsigned int abs = value;
    int div = 10;
    int alpha = 'a';

    /* If specifier is signed, put - in the head for negative values. */
    if (spec == 'd' && (signed int)value < 0) {
        *dest++ = '-';
        dest_len--;
        head++;
        abs = -value;
    } else if (spec == 'x' || spec == 'X' || spec == 'p') {
        div = 16;
        alpha = spec == 'X' ? 'A' : 'a';
    }

    do {
        int rem = abs % div;
        *dest++ = rem < 10 ? '0' + rem : alpha + rem - 10;
        if (!--dest_len)
            goto reverse;
    } while (abs /= div);
reverse:
    *dest = 0;
    dest--;
    while (head < dest) {
        char tmp = *head;
        *head = *dest;
        *dest = tmp;
        head++;
        dest--;
    }
}

int vprintf(const char* restrict fmt, va_list v)
{
    char buf[64];
    char pad_char;
    int pad_len = 0, pad_sign = 1;
    int force_spec = 0;

    for (char c = *fmt++; c != '\0'; c = *fmt++) {
        if (c != '%' && !force_spec) {
            putchar(c);
            continue;
        }
        char spec = *fmt++;
        force_spec = 0;
        switch (spec) {
        case 'c':
            putchar(va_arg(v, int));
            break;
        case 's':
            puts(va_arg(v, const char*));
            break;
        case 'd':
        case 'u':
        case 'x':
        case 'X':
        case 'p': {
            if (spec == 'p') {
                puts("0x");
                pad_char = '0';
                pad_len = 2 * sizeof(unsigned long long int);
            }
            _int2str(buf, spec, va_arg(v, int), sizeof(buf));
            int n = strlen(buf);
            if (pad_len > n) {
                pad_len -= n;
                for (int i = 0; i < pad_len; i++)
                    putchar(pad_char);
            }
            puts(buf);
            if (pad_len < 0) {
                pad_len = -pad_len - n;
                for (ssize_t i = 0; i < pad_len; i++)
                    putchar(pad_char);
            }
            break;
        }
        default: {
            /* Unknown specifier. Probably a padding character. */
            pad_char = spec;
            if (*fmt == '-') {
                fmt++;
                pad_sign = -1;
            }
            fmt += _str2int(fmt, &pad_len);
            pad_len *= pad_sign;
            force_spec = 1;
            break;
        }
        }
    }
    return -1;
}

int vsnprintf(
    char* restrict buf, size_t n, const char* restrict fmt, va_list args)
{
    return -1;
}

int vsprintf(char* restrict buf, const char* restrict fmt, va_list args)
{
    return -1;
}
