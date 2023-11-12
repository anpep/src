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
#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "printf_convspec.h"

static int parse_argno(struct convspec *convspec, const char *fmt)
{
    char *endptr;
    uintmax_t retval;

    errno = 0;
    endptr = NULL;
    retval = strtoumax(fmt, &endptr, 10);
    if (endptr == NULL || (retval == 0 && errno != 0)) {
        /* Not a number -- invalid argno syntax. Skip no characters in fmt. */
        errno = 0;
        return 0;
    }
    if (*endptr != '$') {
        /* Invalid argno syntax (expected $). Skip no characters in fmt. */
        return 0;
    }

    /* Parsed a valid argno syntax. */
    convspec->argno = retval;
    convspec->has_argno = 1;
    /* Skip final $. */
    endptr++;
    /* endptr returned by strtoumax() MUST be a pointer to a character
       within as fmt. Return the number of characters to skip. */
    return (int)((ptrdiff_t)(endptr - fmt));
}

#define SET_FLAG_ONCE(dst, f)                                                  \
    {                                                                          \
        if (((dst) & (f)) != 0U) {                                             \
            /* Flag already set -- return and parse next part. */              \
            return retval;                                                     \
        }                                                                      \
        /* Flag not yet set. */                                                \
        (dst) |= (f);                                                          \
        convspec->has_flags = 1;                                               \
    }

static int parse_flags(struct convspec *convspec, const char *fmt)
{
    int retval = 0;

    for (;; retval++) {
        switch (*fmt) {
        case '#':
            SET_FLAG_ONCE(convspec->flags, CONVSPEC_HASH);
            break;
        case ' ':
            SET_FLAG_ONCE(convspec->flags, CONVSPEC_SPACE);
            break;
        case '+':
            SET_FLAG_ONCE(convspec->flags, CONVSPEC_PLUS);
            break;
        case '0':
            SET_FLAG_ONCE(convspec->flags, CONVSPEC_ZERO);
            break;
        case '-':
            SET_FLAG_ONCE(convspec->flags, CONVSPEC_MINUS);
            break;
        default:
            return retval;
        }
    }

    return retval;
}

static int parse_width(struct convspec *convspec, const char *fmt)
{
    char *endptr;
    uintmax_t retval;

    errno = 0;
    endptr = NULL;
    retval = strtoumax(fmt, &endptr, 10);
    if (endptr == NULL || (retval == 0 && errno != 0)) {
        /* Not a number -- invalid width syntax. Skip no characters in fmt. */
        errno = 0;
        return 0;
    }

    /* Parsed a valid width syntax. */
    convspec->width = retval;
    convspec->has_width = endptr != fmt;
    /* endptr returned by strtoumax() MUST be a pointer to a character
       within as fmt. Return the number of characters to skip. */
    return (int)((ptrdiff_t)(endptr - fmt));
}

static int parse_precision(
    struct convspec *convspec __attribute__((unused)), const char *fmt)
{
    const char *fmt_initial = fmt;
    char *endptr;
    uintmax_t retval;

    if (*fmt != '.') {
        return 0;
    }

    /* Skip initial '.'. */
    fmt++;
    errno = 0;
    endptr = NULL;
    retval = strtoumax(fmt, &endptr, 10);
    if (endptr == NULL || (retval == 0 && errno != 0)) {
        /* Not a number -- invalid width syntax. Skip no characters in fmt. */
        errno = 0;
        return 0;
    }

    /* Parsed a valid width syntax. */
    convspec->prec = retval;
    convspec->has_prec = 1;
    /* endptr returned by strtoumax() MUST be a pointer to a character
       within as fmt. Return the number of characters to skip. */
    return (int)((ptrdiff_t)(endptr - fmt_initial));
}

static int parse_length_modifier(struct convspec *convspec, const char *fmt)
{
    int retval = 0;

    switch (*fmt) {
    case 'h':
        /* h (char) */
        fmt++, retval++;
        convspec->len = CONVSPEC_CHAR;
        if (*fmt == 'h') {
            /* hh (short) */
            retval++;
            convspec->len = CONVSPEC_SHORT;
        }
        break;
    case 'l':
        /* l (long) */
        fmt++, retval++;
        convspec->len = CONVSPEC_LONG;
        if (*fmt == 'l') {
            /* ll (long long) */
            retval++;
            convspec->len = CONVSPEC_LONG_LONG;
        }
        break;
    case 'j':
        fmt++, retval++;
        convspec->len = CONVSPEC_MAX;
        break;
    case 'z':
        fmt++, retval++;
        convspec->len = CONVSPEC_SIZE;
        break;
    case 't':
        fmt++, retval++;
        convspec->len = CONVSPEC_PTRDIFF;
        break;
    case 'L':
        fmt++, retval++;
        convspec->len = CONVSPEC_LONG_DOUBLE;
        break;
    default:
        /* Skip no characters. */
        return 0;
    }

    convspec->has_len = 1;
    return retval;
}

static int parse_specifier(struct convspec *convspec, const char *fmt)
{
    switch (*fmt) {
    case '%':
        /* '%' supports no parts. */
        if (convspec->has_prec || convspec->has_len || convspec->has_argno
            || convspec->has_flags || convspec->has_width) {
            return -1;
        }
        break;

    case 'c':
        /* Check supported parts ('c' only supports argno, flags, len and
         * width). */
        if (convspec->has_prec) {
            return -1;
        }
        /* Check supported flags ('c' only supports '-'). */
        if ((convspec->flags & CONVSPEC_HASH) != 0
            || (convspec->flags & CONVSPEC_SPACE) != 0
            || (convspec->flags & CONVSPEC_PLUS) != 0
            || (convspec->flags & CONVSPEC_ZERO) != 0) {
            return -1;
        }
        /* Check supported modifiers ('c' only supports 'l' or none). */
        if (convspec->len != 0 && convspec->len != CONVSPEC_LONG) {
            return -1;
        }
        break;

    case 's':
        /* Check supported flags ('s' only supports '-'). */
        if ((convspec->flags & CONVSPEC_HASH) != 0
            || (convspec->flags & CONVSPEC_SPACE) != 0
            || (convspec->flags & CONVSPEC_PLUS) != 0
            || (convspec->flags & CONVSPEC_ZERO) != 0) {
            return -1;
        }
        /* Check supported modifiers ('s' only supports 'l' or none). */
        if (convspec->len != 0 && convspec->len != CONVSPEC_LONG) {
            return -1;
        }
        break;

    case 'a':
    case 'A':
    case 'd':
    case 'D':
    case 'e':
    case 'E':
    case 'f':
    case 'F':
    case 'g':
    case 'G':
    case 'i':
    case 'n':
    case 'o':
    case 'O':
    case 'p':
    case 'u':
    case 'U':
    case 'x':
    case 'X':
        /* TODO: Implement this specifier. */
        errno = ENOSYS;
        return -1;

    default:
        /* Invalid or unsupported specifier. */
        return -1;
    }

    convspec->conv = *fmt;
    return 1;
}

int convspec_parse(struct convspec *convspec, const char *fmt)
{
    const char *fmt_initial = fmt;
    int retval = 0;

    convspec->argno = 0;
    convspec->has_argno = 0;

    convspec->flags = 0;
    convspec->has_flags = 0;

    convspec->width = 0;
    convspec->has_width = 0;

    convspec->prec = 0;
    convspec->has_prec = 0;

    convspec->len = 0;
    convspec->has_len = 0;

    convspec->conv = '\0';

    /* Skip until we find a conversion specifier or the end of the string. */
    if (*fmt != '%') {
        while (*fmt != '%' && *fmt != '\0') {
            fmt++, retval++;
        }
        return retval;
    }

    /* Skip initial '%' character. */
    fmt++;

    retval = parse_argno(convspec, fmt);
    if (retval < 0) {
        return retval;
    }
    fmt += retval;

    retval = parse_flags(convspec, fmt);
    if (retval < 0) {
        return retval;
    }
    fmt += retval;

    retval = parse_width(convspec, fmt);
    if (retval < 0) {
        return retval;
    }
    fmt += retval;

    retval = parse_precision(convspec, fmt);
    if (retval < 0) {
        return retval;
    }
    fmt += retval;

    retval = parse_length_modifier(convspec, fmt);
    if (retval < 0) {
        return retval;
    }
    fmt += retval;

    retval = parse_specifier(convspec, fmt);
    if (retval < 0) {
        return retval;
    }
    fmt += retval;

    return (int)((ptrdiff_t)(fmt - fmt_initial));
}
