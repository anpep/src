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

#include <stdint.h>

/* Flags for conversion specifiers. */

/* Use an alternate form for the conversion. The effect of this flag
 * depends on the specifier */
#define CONVSPEC_HASH (1U << 1U)
/* Use a space (' ') before a positive number. */
#define CONVSPEC_SPACE (1U << 2U)
/* Use a plus ('+') before a positive number. */
#define CONVSPEC_PLUS (1U << 3U)
/* Pad numbers with leading zeros instead of spaces. */
#define CONVSPEC_ZERO (1U << 4U)
/* Pad numbers to the left instead of to the right. */
#define CONVSPEC_MINUS (1U << 5U)

enum convspec_length {
    /* Specifies that the specifier applies to a char argument. */
    CONVSPEC_CHAR = 1,
    /* Specifies that the specifier applies to a short argument. */
    CONVSPEC_SHORT = 2,
    /* Specifies that the specifier applies to a long argument. */
    CONVSPEC_LONG = 3,
    /* Specifies that the specifier applies to a long long argument. */
    CONVSPEC_LONG_LONG = 4,
    /* Specifies that the specifier applies to an integer argument of the
     * maximum size in this architecture. */
    CONVSPEC_MAX = 5,
    /* Specifies that the specifier applies to a size_t argument. */
    CONVSPEC_SIZE = 6,
    /* Specifies that the specifier applies to a ptrdiff_t argument. */
    CONVSPEC_PTRDIFF = 7,
    /* Specifies that the specifier applies to a long double argument. */
    CONVSPEC_LONG_DOUBLE = 8,
};

/* Information carried by a format specifier. */
struct convspec {
    /* Index of the next argument to access. */
    uintmax_t argno;
    int has_argno;

    unsigned flags;
    int has_flags;

    /* Minimum field with used for right-adjusting the field with space
     * characters. */
    uintmax_t width;
    int has_width;

    /* For integer conversions, indicates the minimum number of digits.
     * For string conversions, indicates the maximum number of bytes. */
    uintmax_t prec;
    int has_prec;

    /* Argument length modifier. Its effect depends on the specifier. */
    enum convspec_length len;
    int has_len;

    /* Character determining the expected argument type and its format. */
    char conv;
};

/**
 * @brief       Parses a conversion specifier.
 *
 * @param[out]  convspec  A pointer to a @ref convspec struct that will hold
 *                        the parameters for the conversion specifier.
 * @param[in]   fmt       The format string.
 *
 * @return      On error, a negative value. On success, the number of bytes to
 *              be skipped after the parsed conversion specifier. If no
 *              specifier was found, the number of bytes to be skipped until
 *              the next conversion specifier or the NUL terminator are found
 *              (whichever comes first).
 */
int convspec_parse(struct convspec *convspec, const char *fmt);
