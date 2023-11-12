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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../sys/libc/printf_convspec.c"

/* Used to fill convspec struct for testing. */
static const int k_magic_fill = 0xCC;

/* Test state. */
struct state {
    /* Return value of the convspec_parse() function. */
    int retval;
    /* Struct containing information about the parsed conversion specifier. */
    struct convspec convspec;
};

/* Prepares a single test to run. */
static void prepare_test(struct state *state)
{
    assert(state != NULL);
    memset(state, k_magic_fill, sizeof(*state));
}

/* Test that parsing a format string skips until a conversion specifier is
 * found. */
static void test_parse_no_spec(void)
{
    struct state state;
    prepare_test(&state);

    state.retval = convspec_parse(&state.convspec, "hello, %s!");
    assert(state.retval == strlen("hello, "));
    assert(state.convspec.argno == 0);
    assert(state.convspec.flags == 0);
    assert(state.convspec.width == 0);
    assert(state.convspec.prec == 0);
    assert(state.convspec.len == 0);
    assert(state.convspec.conv == '\0');

    state.retval = convspec_parse(&state.convspec, "hello, world!");
    assert(state.retval == strlen("hello, world!"));
    assert(state.convspec.argno == 0);
    assert(state.convspec.flags == 0);
    assert(state.convspec.width == 0);
    assert(state.convspec.prec == 0);
    assert(state.convspec.len == 0);
    assert(state.convspec.conv == '\0');
}

/* Test that parsing the most basic conversion specifier syntax works. */
static void test_parse_simple(void)
{
    struct state state;
    prepare_test(&state);

    state.retval = convspec_parse(&state.convspec, "%c");
    assert(state.retval == strlen("%c"));
    assert(state.convspec.argno == 0);
    assert(state.convspec.flags == 0);
    assert(state.convspec.width == 0);
    assert(state.convspec.prec == 0);
    assert(state.convspec.len == 0);
    assert(state.convspec.conv == 'c');
}

/* Test that parsing length modifiers works. */
static void test_parse_length_modifier(void)
{
    struct state state;
    prepare_test(&state);

    state.retval = convspec_parse(&state.convspec, "%lc");
    assert(state.retval == strlen("%lc"));
    assert(state.convspec.argno == 0);
    assert(state.convspec.flags == 0);
    assert(state.convspec.width == 0);
    assert(state.convspec.prec == 0);
    assert(state.convspec.len == CONVSPEC_LONG);
    assert(state.convspec.conv == 'c');
}

/* Test that parsing width works. */
static void test_parse_width(void)
{
    struct state state;
    prepare_test(&state);

    state.retval = convspec_parse(&state.convspec, "%-1337c");
    assert(state.retval == strlen("%-1337c"));
    assert(state.convspec.argno == 0);
    assert(state.convspec.flags == CONVSPEC_MINUS);
    assert(state.convspec.width == 1337);
    assert(state.convspec.prec == 0);
    assert(state.convspec.len == 0);
    assert(state.convspec.conv == 'c');
}

/* Test that parsing argno works. */
static void test_parse_argno(void)
{
    struct state state;
    prepare_test(&state);

    state.retval = convspec_parse(&state.convspec, "%1337$c");
    assert(state.retval == strlen("%1337$c"));
    assert(state.convspec.argno == 1337);
    assert(state.convspec.flags == 0);
    assert(state.convspec.width == 0);
    assert(state.convspec.prec == 0);
    assert(state.convspec.len == 0);
    assert(state.convspec.conv == 'c');
}

/* Test that parsing the full %% specifier works. */
static void test_parse_full_percent(void)
{
    struct state state;
    prepare_test(&state);

    state.retval = convspec_parse(&state.convspec, "%%");
    assert(state.retval == strlen("%%"));
    assert(state.convspec.argno == 0);
    assert(state.convspec.flags == 0);
    assert(state.convspec.width == 0);
    assert(state.convspec.prec == 0);
    assert(state.convspec.len == 0);
    assert(state.convspec.conv == '%');

    /* The '%' specifiers supports no argno syntax. */
    state.retval = convspec_parse(&state.convspec, "%1$%");
    assert(state.retval < 0);

    /* The '%' specifiers supports no flags syntax. */
    state.retval = convspec_parse(&state.convspec, "%+%");
    assert(state.retval < 0);

    /* The '%' specifiers supports no width syntax. */
    state.retval = convspec_parse(&state.convspec, "%4%");
    assert(state.retval < 0);

    /* The '%' specifiers supports no precision syntax. */
    state.retval = convspec_parse(&state.convspec, "%.4%");
    assert(state.retval < 0);

    /* The '%' specifiers supports no length modifier syntax. */
    state.retval = convspec_parse(&state.convspec, "%l%");
    assert(state.retval < 0);
}

/* Test that parsing the full %c specifier works. */
static void test_parse_full_char(void)
{
    struct state state;
    prepare_test(&state);

    state.retval = convspec_parse(&state.convspec, "%1337$-42lc");
    assert(state.retval == strlen("%1337$-42lc"));
    assert(state.convspec.argno == 1337);
    assert(state.convspec.flags == CONVSPEC_MINUS);
    assert(state.convspec.width == 42);
    assert(state.convspec.prec == 0);
    assert(state.convspec.len == CONVSPEC_LONG);
    assert(state.convspec.conv == 'c');

    /* The 'c' specifier does not support the '#', ' ', '+' or '0' flags. */
    state.retval = convspec_parse(&state.convspec, "%1337$#42c");
    assert(state.retval < 0);
    state.retval = convspec_parse(&state.convspec, "%1337$ 42c");
    assert(state.retval < 0);
    state.retval = convspec_parse(&state.convspec, "%1337$+42c");
    assert(state.retval < 0);
    state.retval = convspec_parse(&state.convspec, "%1337$042c");
    assert(state.retval < 0);

    /* The 'c' specifier supports no precision. */
    state.retval = convspec_parse(&state.convspec, "%.2c");
    assert(state.retval < 0);

    /* The 'c' specifier only supports 'l' length modifier (or none). */
    state.retval = convspec_parse(&state.convspec, "%hc");
    assert(state.retval < 0);
    state.retval = convspec_parse(&state.convspec, "%hhc");
    assert(state.retval < 0);
    state.retval = convspec_parse(&state.convspec, "%jc");
    assert(state.retval < 0);
    state.retval = convspec_parse(&state.convspec, "%zc");
    assert(state.retval < 0);
    state.retval = convspec_parse(&state.convspec, "%tc");
    assert(state.retval < 0);
    state.retval = convspec_parse(&state.convspec, "%Lc");
    assert(state.retval < 0);
}

/* Test that parsing the full %s specifier works. */
static void test_parse_full_string(void)
{
    struct state state;
    prepare_test(&state);

    state.retval = convspec_parse(&state.convspec, "%1337$-42.512ls");
    assert(state.retval == strlen("%1337$-42.512ls"));
    assert(state.convspec.argno == 1337);
    assert(state.convspec.flags == CONVSPEC_MINUS);
    assert(state.convspec.width == 42);
    assert(state.convspec.prec == 512);
    assert(state.convspec.len == CONVSPEC_LONG);
    assert(state.convspec.conv == 's');

    /* The 's' specifier does not support the '#', ' ', '+' or '0' flags. */
    state.retval = convspec_parse(&state.convspec, "%1337$#42s");
    assert(state.retval < 0);
    state.retval = convspec_parse(&state.convspec, "%1337$ 42s");
    assert(state.retval < 0);
    state.retval = convspec_parse(&state.convspec, "%1337$+42s");
    assert(state.retval < 0);
    state.retval = convspec_parse(&state.convspec, "%1337$042s");
    assert(state.retval < 0);

    /* The 'c' specifier only supports 'l' length modifier (or none). */
    state.retval = convspec_parse(&state.convspec, "%hs");
    assert(state.retval < 0);
    state.retval = convspec_parse(&state.convspec, "%hhs");
    assert(state.retval < 0);
    state.retval = convspec_parse(&state.convspec, "%js");
    assert(state.retval < 0);
    state.retval = convspec_parse(&state.convspec, "%zs");
    assert(state.retval < 0);
    state.retval = convspec_parse(&state.convspec, "%ts");
    assert(state.retval < 0);
    state.retval = convspec_parse(&state.convspec, "%Ls");
    assert(state.retval < 0);
}

int main(int argc __attribute__((unused)), char **argv __attribute__((unused)))
{
    test_parse_no_spec();

    test_parse_simple();
    test_parse_length_modifier();
    test_parse_width();
    test_parse_argno();

    test_parse_full_percent();
    test_parse_full_char();
    test_parse_full_string();

    return 0;
}
