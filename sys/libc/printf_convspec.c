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
    uintmax_t rc;

    errno = 0;
    endptr = NULL;
    rc = strtoumax(fmt, &endptr, 10);
    if (endptr == NULL || (rc == 0 && errno != 0)) {
        /* Not a number -- invalid argno syntax. Skip no characters in fmt. */
        errno = 0;
        return 0;
    }
    if (*endptr != '$') {
        /* Invalid argno syntax (expected $). Skip no characters in fmt. */
        return 0;
    }

    /* Parsed a valid argno syntax. */
    convspec->argno = rc;
    convspec->has_argno = 1;
    /* Skip final $. */
    endptr++;
    /* endptr returned by strtoumax() MUST be a pointer to a character
       within as fmt. Return the number of characters to skip. */
    return (int)((ptrdiff_t)(endptr - fmt));
}

#define SET_FLAG_ONCE(dst, f)                                                  \
    {                                                                          \
        if (((dst) & (f)) != 0) {                                              \
            /* Flag already set -- return and parse next part. */              \
            return rc;                                                         \
        }                                                                      \
        /* Flag not yet set. */                                                \
        (dst) |= (f);                                                          \
        convspec->has_flags = 1;                                               \
    }

static int parse_flags(struct convspec *convspec, const char *fmt)
{
    int rc = 0;

    for (;; rc++) {
        switch (*fmt) {
        case '#':
            SET_FLAG_ONCE(convspec->flags, CONVSPEC_HASH);
            fmt++;
            break;
        case ' ':
            SET_FLAG_ONCE(convspec->flags, CONVSPEC_SPACE);
            fmt++;
            break;
        case '+':
            SET_FLAG_ONCE(convspec->flags, CONVSPEC_PLUS);
            fmt++;
            break;
        case '0':
            SET_FLAG_ONCE(convspec->flags, CONVSPEC_ZERO);
            fmt++;
            break;
        case '-':
            SET_FLAG_ONCE(convspec->flags, CONVSPEC_MINUS);
            fmt++;
            break;
        default:
            return rc;
        }
    }

    return rc;
}

static int parse_width(struct convspec *convspec, const char *fmt)
{
    char *endptr;
    uintmax_t rc;

    errno = 0;
    endptr = NULL;
    rc = strtoumax(fmt, &endptr, 10);
    if (endptr == NULL || (rc == 0 && errno != 0)) {
        /* Not a number -- invalid width syntax. Skip no characters in fmt. */
        errno = 0;
        return 0;
    }

    /* Parsed a valid width syntax. */
    convspec->width = rc;
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
    uintmax_t rc;

    if (*fmt != '.') {
        return 0;
    }

    /* Skip initial '.'. */
    fmt++;
    errno = 0;
    endptr = NULL;
    rc = strtoumax(fmt, &endptr, 10);
    if (endptr == NULL || (rc == 0 && errno != 0)) {
        /* Not a number -- invalid width syntax. Skip no characters in fmt. */
        errno = 0;
        return 0;
    }

    /* Parsed a valid width syntax. */
    convspec->prec = rc;
    convspec->has_prec = 1;
    /* endptr returned by strtoumax() MUST be a pointer to a character
       within as fmt. Return the number of characters to skip. */
    return (int)((ptrdiff_t)(endptr - fmt_initial));
}

static int parse_length_modifier(struct convspec *convspec, const char *fmt)
{
    int rc = 0;

    switch (*fmt) {
    case 'h':
        /* h (short) */
        fmt++, rc++;
        convspec->len = CONVSPEC_SHORT;
        if (*fmt == 'h') {
            /* hh (char) */
            rc++;
            convspec->len = CONVSPEC_CHAR;
        }
        break;
    case 'l':
        /* l (long) */
        fmt++, rc++;
        convspec->len = CONVSPEC_LONG;
        if (*fmt == 'l') {
            /* ll (long long) */
            rc++;
            convspec->len = CONVSPEC_LONG_LONG;
        }
        break;
    case 'j':
        fmt++, rc++;
        convspec->len = CONVSPEC_MAX;
        break;
    case 'z':
        fmt++, rc++;
        convspec->len = CONVSPEC_SIZE;
        break;
    case 't':
        fmt++, rc++;
        convspec->len = CONVSPEC_PTRDIFF;
        break;
    case 'L':
        fmt++, rc++;
        convspec->len = CONVSPEC_LONG_DOUBLE;
        break;
    default:
        /* Skip no characters. */
        return 0;
    }

    convspec->has_len = 1;
    return rc;
}

static int parse_specifier(struct convspec *cs, const char *fmt)
{
    switch (*fmt) {
    case '%':
        /* '%' supports no parts. */
        if (cs->has_prec || cs->has_len || cs->has_argno || cs->has_flags
            || cs->has_width) {
            return -1;
        }
        break;

    case 'c':
        /* Check supported parts ('c' only supports argno, flags, len and
         * width). */
        if (cs->has_prec) {
            return -1;
        }
        /* Check supported flags ('c' only supports '-'). */
        if (cs->flags & ~CONVSPEC_MINUS) {
            return -1;
        }
        /* Check supported modifiers ('c' only supports 'l' or none). */
        if (cs->len != 0 && cs->len != CONVSPEC_LONG) {
            return -1;
        }
        break;

    case 's':
        /* Check supported flags ('s' only supports '-'). */
        if (cs->flags & ~CONVSPEC_MINUS) {
            return -1;
        }
        /* Check supported modifiers ('s' only supports 'l' or none). */
        if (cs->len != 0 && cs->len != CONVSPEC_LONG) {
            return -1;
        }
        break;

    case 'd':
        if (cs->flags & CONVSPEC_HASH) {
            /* '#' is not supported for 'd' specifier. */
            return -1;
        }

        /* ' ' and '+' are mutually exclusive for 'd' specifier. */
        if ((cs->flags & (CONVSPEC_SPACE | CONVSPEC_PLUS))
            == (CONVSPEC_SPACE | CONVSPEC_PLUS)) {
            return -1;
        }
        /* '-' and '0' are mutually exclusive for 'd' specifier. */
        if ((cs->flags & (CONVSPEC_MINUS | CONVSPEC_ZERO))
            == (CONVSPEC_MINUS | CONVSPEC_ZERO)) {
            return -1;
        }
        /* Check supported modifiers ('L' is not supported). */
        if (cs->len == CONVSPEC_LONG_DOUBLE) {
            return -1;
        }
        break;

    case 'u':
        if ((cs->flags & (CONVSPEC_HASH | CONVSPEC_SPACE | CONVSPEC_PLUS))
            != 0) {
            /* '#', ' ' and '+' are not supported for 'u' specifier. */
            return -1;
        }
        /* '-' and '0' are mutually exclusive for 'u' specifier. */
        if ((cs->flags & (CONVSPEC_MINUS | CONVSPEC_ZERO))
            == (CONVSPEC_MINUS | CONVSPEC_ZERO)) {
            return -1;
        }
        /* Check supported modifiers ('L' is not supported). */
        if (cs->len == CONVSPEC_LONG_DOUBLE) {
            return -1;
        }
        break;

    case 'a':
    case 'A':
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

    cs->conv = *fmt;
    return 1;
}

int convspec_parse(struct convspec *cs, const char *fmt)
{
    const char *fmt_initial = fmt;
    int rc = 0;

    cs->argno = 0;
    cs->has_argno = 0;

    cs->flags = 0;
    cs->has_flags = 0;

    cs->width = 0;
    cs->has_width = 0;

    cs->prec = 0;
    cs->has_prec = 0;

    cs->len = 0;
    cs->has_len = 0;

    cs->conv = '\0';

    /* Skip until we find a conversion specifier or the end of the string. */
    if (*fmt != '%') {
        while (*fmt != '%' && *fmt != '\0') {
            fmt++, rc++;
        }
        return rc;
    }

    /* Skip initial '%' character. */
    fmt++;

    rc = parse_argno(cs, fmt);
    if (rc < 0) {
        return rc;
    }
    fmt += rc;

    rc = parse_flags(cs, fmt);
    if (rc < 0) {
        return rc;
    }
    fmt += rc;

    rc = parse_width(cs, fmt);
    if (rc < 0) {
        return rc;
    }
    fmt += rc;

    rc = parse_precision(cs, fmt);
    if (rc < 0) {
        return rc;
    }
    fmt += rc;

    rc = parse_length_modifier(cs, fmt);
    if (rc < 0) {
        return rc;
    }
    fmt += rc;

    rc = parse_specifier(cs, fmt);
    if (rc < 0) {
        return rc;
    }
    fmt += rc;

    return (int)((ptrdiff_t)(fmt - fmt_initial));
}
