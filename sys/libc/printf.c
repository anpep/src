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

#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <sys/param.h>
#include <unistd.h>

#include "printf_convspec.h"

/* Prototype of the write() system call. */
typedef ssize_t (*write_t)(int, const void *, size_t);
struct vnprintf_opts {
    int is_bounded; /* Whether this was a *nprintf() function or not. */
    size_t n; /* Maximum number of bytes to be written. */
    int fd; /* Destination file descriptor. */
    write_t write; /* Pointer to a write() implementation. */
};

struct pad_opts {
    /* Type of padding to perform. */
    enum { LEADING, TRAILING } type;
    /* Real length of the argument after conversion. */
    size_t len;
};

static ssize_t pad(struct convspec *convspec, struct pad_opts pad_opts,
    struct vnprintf_opts *print_opts)
{
    ssize_t nwritten = 0;
    ssize_t rc;
    char fill;

    if (pad_opts.type == LEADING && (convspec->flags & CONVSPEC_MINUS) != 0) {
        /* Minus flag has been specified (pad characters to the right). */
        return 0;
    }
    if (pad_opts.type == TRAILING && (convspec->flags & CONVSPEC_MINUS) == 0) {
        /* Minus flag was not specified (no trailing padding to be done). */
        return 0;
    }
    if (convspec->width == 0 || pad_opts.len > convspec->width) {
        /* No padding to be done, or text overflows. */
        return 0;
    }
    fill = (convspec->flags & CONVSPEC_ZERO) != 0 ? '0' : ' ';
    /* (arg_len < cs->width) is guaranteed here. */
    for (size_t pad_len = convspec->width - pad_opts.len; pad_len > 0;
         pad_len--) {
        rc = print_opts->write(print_opts->fd, &fill, 1);
        if (rc < 0) {
            return rc;
        }
        nwritten += rc;
    }
    return nwritten;
}

ssize_t vfprintf_impl(
    const char *restrict fmt, va_list args, struct vnprintf_opts opts)
{
    int nskip;
    ssize_t nwritten = 0;
    struct convspec convspec;

    while ((nskip = convspec_parse(&convspec, fmt)) > 0) {
        ssize_t rc;
        void *buf = NULL;
        size_t len = 0;

        if (convspec.conv == '\0') {
            /* No conversion specifier, meaning this was a string. */
            buf = (void *)fmt;
            len = nskip;
        } else if (convspec.conv == 'c') {
            /* Write a single character. */
            char chr = (char)(va_arg(args, unsigned) & 0xFFU);
            buf = &chr;
            len = sizeof(chr);
        } else if (convspec.conv == 's') {
            /* Write a string. */
            buf = va_arg(args, char *);
            len = strlen((char *)buf);
            if (convspec.has_prec) {
                /* Precision part in the 's' conversion specifier determines
                 * the maximum number of bytes from the string to be written
                 * to the output stream. Limit the argument length to that. */
                len = MIN(len, convspec.prec);
            }
        } else if (convspec.conv == 'd') {
            /* Write a signed integer. */
            intmax_t val;
            if (convspec.len == CONVSPEC_LONG) {
                val = va_arg(args, long);
            } else if (convspec.len == CONVSPEC_LONG_LONG) {
                val = va_arg(args, long long);
            } else if (convspec.len == CONVSPEC_MAX) {
                val = va_arg(args, intmax_t);
            } else if (convspec.len == CONVSPEC_SIZE) {
                val = va_arg(args, size_t);
            } else if (convspec.len == CONVSPEC_PTRDIFF) {
                val = va_arg(args, ptrdiff_t);
            } else {
                val = va_arg(args, int);
            }

            char conv[64];
            buf = &conv;

            if (val == 0) {
                conv[len++] = '0';
            } else if (val > 0) {
                if ((convspec.flags & CONVSPEC_PLUS) != 0) {
                    /* Print plus sign before positive number. */
                    conv[len++] = '+';
                } else if ((convspec.flags & CONVSPEC_SPACE) != 0) {
                    /* Print space before positive number. */
                    conv[len++] = ' ';
                }
            } else {
                /* Number is negative */
                conv[len++] = '-';
                val = -val;
            }

            char rev[64];
            size_t n;
            for (n = 0; val > 0; val /= 10) {
                rev[n++] = (char)('0' + (val % 10));
            }
            while (n-- > 0) {
                conv[len++] = rev[n];
            }
            conv[len] = '\0';
        }

        rc = pad(&convspec, (struct pad_opts) { .type = LEADING, .len = len },
            &opts);
        if (rc < 0) {
            goto end;
        }
        nwritten += rc;
        rc = opts.write(opts.fd, buf, len);
        if (rc < 0) {
            goto end;
        }
        nwritten += rc;
        rc = pad(&convspec, (struct pad_opts) { .type = TRAILING, .len = len },
            &opts);
        if (rc < 0) {
            goto end;
        }
        nwritten += rc;
        fmt += nskip;
    }
end:
    return nwritten;
}

int vprintf(const char *restrict fmt, va_list args)
{
    return (int)vfprintf_impl(fmt, args,
        (struct vnprintf_opts) {
            .is_bounded = 0,
            .n = 0,
            .fd = 0,
            .write = write,
        });
}

int uut_printf(const char *restrict fmt, ...)
{
    va_list args;
    int rc;

    va_start(args, fmt);
    rc = vprintf(fmt, args);
    va_end(args);

    return rc;
}
