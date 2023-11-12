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

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

struct strtoint_opts {
    intmax_t min;
    uintmax_t max;
};

static int parse_base(char *nptr, int *base)
{
    int retval = 0;

    if (*nptr == '0') {
        nptr++, retval++;
        if (*nptr == 'x' || *nptr == 'X') {
            /* Hexadecimal base specified by preceding "0x" or "0X".*/
            nptr++, retval++;
            *base = 16;
        } else if (isdigit(*nptr)) {
            /* Octal base specified by preceding "0". */
            *base = 8;
        }
    } else if (isdigit(*nptr)) {
        /* Decimal base specified by at least one preceding non-zero decimal
         * digit. */
        *base = 10;
    } else {
        /* Could not guess base (attempted an invalid conversion). */
        *base = 0;
    }

    return retval;
}

static intmax_t strtoint(const char *restrict nptr, char **restrict endptr,
    int base, struct strtoint_opts opts)
{
    int is_signed = (opts.min < 0);
    int sign = +1;

    while (isspace(*nptr)) {
        /* Skip whitespace. */
        nptr++;
    }

    if (*nptr == '-') {
        if (!is_signed) {
            /* Function does not allow negative values. */
            if (endptr != NULL) {
                *endptr = (char *)nptr;
            }
            errno = EINVAL;
            return 0;
        }
        sign = -1;
        nptr++;
    } else if (*nptr == '+') {
        nptr++;
    }

    if (base == 0) {
        /* Try to guess the base. */
        nptr += parse_base((char *)nptr, &base);
        if (base == 0) {
            /* Could not guess base. */
            if (endptr != NULL) {
                *endptr = (char *)nptr;
            }
            errno = EINVAL;
            return 0;
        }
    } else if (base < 2 || base > 36) {
        /* Invalid value for base. */
        errno = EINVAL;
        return 0;
    }

    intmax_t val = 0;
    for (int digit;; nptr++) {
        if (isdigit(*nptr)) {
            digit = *nptr - '0';
        } else if (*nptr >= 'a' && *nptr <= ('a' + base - 1)) {
            digit = *nptr - 'a' + 10;
        } else if (*nptr >= 'A' && *nptr <= ('A' + base - 1)) {
            digit = *nptr - 'A' + 10;
        } else {
            /* Invalid value for the digit -- finished conversion. */
            if (endptr != NULL) {
                *endptr = (char *)nptr;
            }
            break;
        }
        if (sign == +1 && (intmax_t)(base * val + digit) < (intmax_t)val) {
            errno = ERANGE;
            if (endptr != NULL) {
                *endptr = (char *)nptr;
            }
            return (intmax_t)opts.max;
        }
        if (sign == -1 && (intmax_t)(base * val + digit) < (intmax_t)val) {
            errno = ERANGE;
            if (endptr != NULL) {
                *endptr = (char *)nptr;
            }
            return opts.min;
        }
        val = base * val + digit;
    }

    if (endptr != NULL) {
        *endptr = (char *)nptr;
    }
    return sign * val;
}

long int strtol(const char *restrict nptr, char **restrict endptr, int base)
{
    return (long int)strtoint(nptr, endptr, base,
        (struct strtoint_opts) {
            .min = LONG_MIN,
            .max = LONG_MAX,
        });
}

long long int strtoll(
    const char *restrict nptr, char **restrict endptr, int base)
{
    return (long long int)strtoint(nptr, endptr, base,
        (struct strtoint_opts) {
            .min = LLONG_MIN,
            .max = LLONG_MAX,
        });
}

unsigned long int strtoul(
    const char *restrict nptr, char **restrict endptr, int base)
{
    return strtoint(nptr, endptr, base,
        (struct strtoint_opts) {
            .min = 0,
            .max = ULONG_MAX,
        });
}

unsigned long long int strtoull(
    const char *restrict nptr, char **restrict endptr, int base)
{
    return strtoint(nptr, endptr, base,
        (struct strtoint_opts) {
            .min = 0,
            .max = ULLONG_MAX,
        });
}

intmax_t strtoimax(const char *restrict nptr, char **restrict endptr, int base)
{
    return strtoint(nptr, endptr, base,
        (struct strtoint_opts) {
            .min = INTMAX_MIN,
            .max = INTMAX_MAX,
        });
}

uintmax_t strtoumax(const char *restrict nptr, char **restrict endptr, int base)
{
    return (uintmax_t)strtoint(nptr, endptr, base,
        (struct strtoint_opts) {
            .min = 0,
            .max = UINTMAX_MAX,
        });
}
