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

#include "../../lib/libc/printf_convspec.c"

/* Test state. */
struct state {
    /* Return value of the convspec_parse() function. */
    int rc;
    /* Struct containing information about the parsed conversion specifier. */
    struct convspec cs;
};

/* Prepares a single test to run. */
static void prepare_test(struct state *s)
{
    assert(s != NULL);
    /* Fill with a sentinel value so we catch improper initializations. */
    memset(s, 0xcc, sizeof(*s));
}

/* Test that parsing a format string skips until a conversion specifier is
 * found. */
static void test_parse_no_spec(void)
{
    struct state s;
    prepare_test(&s);

    s.rc = convspec_parse(&s.cs, "hello, %s!");
    assert(s.rc == strlen("hello, "));
    assert(s.cs.argno == 0);
    assert(s.cs.flags == 0);
    assert(s.cs.width == 0);
    assert(s.cs.prec == 0);
    assert(s.cs.len == 0);
    assert(s.cs.conv == '\0');

    s.rc = convspec_parse(&s.cs, "hello, world!");
    assert(s.rc == strlen("hello, world!"));
    assert(s.cs.argno == 0);
    assert(s.cs.flags == 0);
    assert(s.cs.width == 0);
    assert(s.cs.prec == 0);
    assert(s.cs.len == 0);
    assert(s.cs.conv == '\0');
}

/* Test that parsing the most basic conversion specifier syntax works. */
static void test_parse_simple(void)
{
    struct state s;
    prepare_test(&s);

    s.rc = convspec_parse(&s.cs, "%c");
    assert(s.rc == strlen("%c"));
    assert(s.cs.argno == 0);
    assert(s.cs.flags == 0);
    assert(s.cs.width == 0);
    assert(s.cs.prec == 0);
    assert(s.cs.len == 0);
    assert(s.cs.conv == 'c');
}

/* Test that parsing length modifiers works. */
static void test_parse_length_modifier(void)
{
    struct state s;
    prepare_test(&s);

    s.rc = convspec_parse(&s.cs, "%lc");
    assert(s.rc == strlen("%lc"));
    assert(s.cs.argno == 0);
    assert(s.cs.flags == 0);
    assert(s.cs.width == 0);
    assert(s.cs.prec == 0);
    assert(s.cs.len == CONVSPEC_LONG);
    assert(s.cs.conv == 'c');
}

/* Test that parsing width works. */
static void test_parse_width(void)
{
    struct state s;
    prepare_test(&s);

    s.rc = convspec_parse(&s.cs, "%-1337c");
    assert(s.rc == strlen("%-1337c"));
    assert(s.cs.argno == 0);
    assert(s.cs.flags == CONVSPEC_MINUS);
    assert(s.cs.width == 1337);
    assert(s.cs.prec == 0);
    assert(s.cs.len == 0);
    assert(s.cs.conv == 'c');
}

/* Test that parsing argno works. */
static void test_parse_argno(void)
{
    struct state s;
    prepare_test(&s);

    s.rc = convspec_parse(&s.cs, "%1337$c");
    assert(s.rc == strlen("%1337$c"));
    assert(s.cs.argno == 1337);
    assert(s.cs.flags == 0);
    assert(s.cs.width == 0);
    assert(s.cs.prec == 0);
    assert(s.cs.len == 0);
    assert(s.cs.conv == 'c');
}

/* Test that parsing the full %% specifier works. */
static void test_parse_full_percent(void)
{
    struct state s;
    prepare_test(&s);

    s.rc = convspec_parse(&s.cs, "%%");
    assert(s.rc == strlen("%%"));
    assert(s.cs.argno == 0);
    assert(s.cs.flags == 0);
    assert(s.cs.width == 0);
    assert(s.cs.prec == 0);
    assert(s.cs.len == 0);
    assert(s.cs.conv == '%');

    /* The '%' specifiers supports no argno syntax. */
    s.rc = convspec_parse(&s.cs, "%1$%");
    assert(s.rc < 0);

    /* The '%' specifiers supports no flags syntax. */
    s.rc = convspec_parse(&s.cs, "%+%");
    assert(s.rc < 0);

    /* The '%' specifiers supports no width syntax. */
    s.rc = convspec_parse(&s.cs, "%4%");
    assert(s.rc < 0);

    /* The '%' specifiers supports no precision syntax. */
    s.rc = convspec_parse(&s.cs, "%.4%");
    assert(s.rc < 0);

    /* The '%' specifiers supports no length modifier syntax. */
    s.rc = convspec_parse(&s.cs, "%l%");
    assert(s.rc < 0);
}

/* Test that parsing the full %c specifier works. */
static void test_parse_full_char(void)
{
    struct state s;
    prepare_test(&s);

    s.rc = convspec_parse(&s.cs, "%1337$-42lc");
    assert(s.rc == strlen("%1337$-42lc"));
    assert(s.cs.argno == 1337);
    assert(s.cs.flags == CONVSPEC_MINUS);
    assert(s.cs.width == 42);
    assert(s.cs.prec == 0);
    assert(s.cs.len == CONVSPEC_LONG);
    assert(s.cs.conv == 'c');

    /* The 'c' specifier does not support the '#', ' ', '+' or '0' flags. */
    s.rc = convspec_parse(&s.cs, "%1337$#42c");
    assert(s.rc < 0);
    s.rc = convspec_parse(&s.cs, "%1337$ 42c");
    assert(s.rc < 0);
    s.rc = convspec_parse(&s.cs, "%1337$+42c");
    assert(s.rc < 0);
    s.rc = convspec_parse(&s.cs, "%1337$042c");
    assert(s.rc < 0);

    /* The 'c' specifier supports no precision. */
    s.rc = convspec_parse(&s.cs, "%.2c");
    assert(s.rc < 0);

    /* The 'c' specifier only supports 'l' length modifier (or none). */
    s.rc = convspec_parse(&s.cs, "%hc");
    assert(s.rc < 0);
    s.rc = convspec_parse(&s.cs, "%hhc");
    assert(s.rc < 0);
    s.rc = convspec_parse(&s.cs, "%jc");
    assert(s.rc < 0);
    s.rc = convspec_parse(&s.cs, "%zc");
    assert(s.rc < 0);
    s.rc = convspec_parse(&s.cs, "%tc");
    assert(s.rc < 0);
    s.rc = convspec_parse(&s.cs, "%Lc");
    assert(s.rc < 0);
}

/* Test that parsing the full %s specifier works. */
static void test_parse_full_string(void)
{
    struct state s;
    prepare_test(&s);

    s.rc = convspec_parse(&s.cs, "%1337$-42.512ls");
    assert(s.rc == strlen("%1337$-42.512ls"));
    assert(s.cs.argno == 1337);
    assert(s.cs.flags == CONVSPEC_MINUS);
    assert(s.cs.width == 42);
    assert(s.cs.prec == 512);
    assert(s.cs.len == CONVSPEC_LONG);
    assert(s.cs.conv == 's');

    /* The 's' specifier does not support the '#', ' ', '+' or '0' flags. */
    s.rc = convspec_parse(&s.cs, "%1337$#42s");
    assert(s.rc < 0);
    s.rc = convspec_parse(&s.cs, "%1337$ 42s");
    assert(s.rc < 0);
    s.rc = convspec_parse(&s.cs, "%1337$+42s");
    assert(s.rc < 0);
    s.rc = convspec_parse(&s.cs, "%1337$042s");
    assert(s.rc < 0);

    /* The 's' specifier only supports 'l' length modifier (or none). */
    s.rc = convspec_parse(&s.cs, "%hs");
    assert(s.rc < 0);
    s.rc = convspec_parse(&s.cs, "%hhs");
    assert(s.rc < 0);
    s.rc = convspec_parse(&s.cs, "%js");
    assert(s.rc < 0);
    s.rc = convspec_parse(&s.cs, "%zs");
    assert(s.rc < 0);
    s.rc = convspec_parse(&s.cs, "%ts");
    assert(s.rc < 0);
    s.rc = convspec_parse(&s.cs, "%Ls");
    assert(s.rc < 0);
}

/* Test that parsing the full %d specifier works. */
static void test_parse_full_signed(void)
{
    struct state s;
    prepare_test(&s);

    s.rc = convspec_parse(&s.cs, "%1337$-42.512hhd");
    assert(s.rc == strlen("%1337$-42.512hhd"));
    assert(s.cs.argno == 1337);
    assert(s.cs.flags == CONVSPEC_MINUS);
    assert(s.cs.width == 42);
    assert(s.cs.prec == 512);
    assert(s.cs.len == CONVSPEC_CHAR);
    assert(s.cs.conv == 'd');

    /* The 'd' specifier does not support '#'. */
    s.rc = convspec_parse(&s.cs, "%#d");
    assert(s.rc < 0);
    /* The 'd' specifier does not support ' ' and '+' simultaneously. */
    s.rc = convspec_parse(&s.cs, "% +d");
    assert(s.rc < 0);
    /* The 'd' specifier does not support '-' and '0' simultaneously. */
    s.rc = convspec_parse(&s.cs, "%-0d");
    assert(s.rc < 0);
    /* The 'd' specifier supports ' ' and '-' simultaneously. */
    s.rc = convspec_parse(&s.cs, "% -d");
    assert(s.rc == strlen("% -d"));
    assert(s.cs.flags == (CONVSPEC_SPACE | CONVSPEC_MINUS));
    /* The 'd' specifier supports '+' and '0' simultaneously. */
    s.rc = convspec_parse(&s.cs, "%+0d");
    assert(s.rc == strlen("%+0d"));
    assert(s.cs.flags == (CONVSPEC_PLUS | CONVSPEC_ZERO));

    /* The 'd' specifier supports all length modifiers but 'L'. */
    s.rc = convspec_parse(&s.cs, "%hhd");
    assert(s.rc == strlen("%hhd"));
    assert(s.cs.has_len == 1);
    assert(s.cs.len == CONVSPEC_CHAR);
    s.rc = convspec_parse(&s.cs, "%hd");
    assert(s.rc == strlen("%hd"));
    assert(s.cs.has_len == 1);
    assert(s.cs.len == CONVSPEC_SHORT);
    s.rc = convspec_parse(&s.cs, "%d");
    assert(s.rc == strlen("%d"));
    assert(s.cs.has_len == 0);
    assert(s.cs.len == 0);
    s.rc = convspec_parse(&s.cs, "%ld");
    assert(s.rc == strlen("%ld"));
    assert(s.cs.has_len == 1);
    assert(s.cs.len == CONVSPEC_LONG);
    s.rc = convspec_parse(&s.cs, "%lld");
    assert(s.rc == strlen("%lld"));
    assert(s.cs.has_len == 1);
    assert(s.cs.len == CONVSPEC_LONG_LONG);
    s.rc = convspec_parse(&s.cs, "%jd");
    assert(s.rc == strlen("%jd"));
    assert(s.cs.has_len == 1);
    assert(s.cs.len == CONVSPEC_MAX);
    s.rc = convspec_parse(&s.cs, "%td");
    assert(s.rc == strlen("%td"));
    assert(s.cs.has_len == 1);
    assert(s.cs.len == CONVSPEC_PTRDIFF);
    s.rc = convspec_parse(&s.cs, "%zd");
    assert(s.rc == strlen("%zd"));
    assert(s.cs.has_len == 1);
    assert(s.cs.len == CONVSPEC_SIZE);
    s.rc = convspec_parse(&s.cs, "%Ld");
    assert(s.rc < 0);
}

/* Test that parsing the full %u specifier works. */
static void test_parse_full_unsigned(void)
{
    struct state s;
    prepare_test(&s);

    s.rc = convspec_parse(&s.cs, "%1337$-42.512hhu");
    assert(s.rc == strlen("%1337$-42.512hhu"));
    assert(s.cs.argno == 1337);
    assert(s.cs.flags == CONVSPEC_MINUS);
    assert(s.cs.width == 42);
    assert(s.cs.prec == 512);
    assert(s.cs.len == CONVSPEC_CHAR);
    assert(s.cs.conv == 'u');

    /* The 'u' specifier does not support '#', ' ' or '+'. */
    s.rc = convspec_parse(&s.cs, "%#u");
    assert(s.rc < 0);
    s.rc = convspec_parse(&s.cs, "% u");
    assert(s.rc < 0);
    s.rc = convspec_parse(&s.cs, "%+u");
    assert(s.rc < 0);
    /* The 'u' specifier does not support '-' and '0' simultaneously. */
    s.rc = convspec_parse(&s.cs, "%-0u");
    assert(s.rc < 0);
    /* The 'u' specifier supports all length modifiers but 'L'. */
    s.rc = convspec_parse(&s.cs, "%hhu");
    assert(s.rc == strlen("%hhu"));
    assert(s.cs.has_len == 1);
    assert(s.cs.len == CONVSPEC_CHAR);
    s.rc = convspec_parse(&s.cs, "%hu");
    assert(s.rc == strlen("%hu"));
    assert(s.cs.has_len == 1);
    assert(s.cs.len == CONVSPEC_SHORT);
    s.rc = convspec_parse(&s.cs, "%u");
    assert(s.rc == strlen("%u"));
    assert(s.cs.has_len == 0);
    assert(s.cs.len == 0);
    s.rc = convspec_parse(&s.cs, "%lu");
    assert(s.rc == strlen("%lu"));
    assert(s.cs.has_len == 1);
    assert(s.cs.len == CONVSPEC_LONG);
    s.rc = convspec_parse(&s.cs, "%llu");
    assert(s.rc == strlen("%llu"));
    assert(s.cs.has_len == 1);
    assert(s.cs.len == CONVSPEC_LONG_LONG);
    s.rc = convspec_parse(&s.cs, "%ju");
    assert(s.rc == strlen("%ju"));
    assert(s.cs.has_len == 1);
    assert(s.cs.len == CONVSPEC_MAX);
    s.rc = convspec_parse(&s.cs, "%tu");
    assert(s.rc == strlen("%tu"));
    assert(s.cs.has_len == 1);
    assert(s.cs.len == CONVSPEC_PTRDIFF);
    s.rc = convspec_parse(&s.cs, "%zu");
    assert(s.rc == strlen("%zu"));
    assert(s.cs.has_len == 1);
    assert(s.cs.len == CONVSPEC_SIZE);
    s.rc = convspec_parse(&s.cs, "%Lu");
    assert(s.rc < 0);
}

/* Test that parsing the full %x specifier works. */
static void test_parse_full_hex(void)
{
    struct state s;
    prepare_test(&s);

    s.rc = convspec_parse(&s.cs, "%1337$#-42.512hhx");
    assert(s.rc == strlen("%1337$#-42.512hhx"));
    assert(s.cs.argno == 1337);
    assert(s.cs.flags == (CONVSPEC_HASH | CONVSPEC_MINUS));
    assert(s.cs.width == 42);
    assert(s.cs.prec == 512);
    assert(s.cs.len == CONVSPEC_CHAR);
    assert(s.cs.conv == 'x');

    /* The 'x' specifier does not support ' ' or '+'. */
    s.rc = convspec_parse(&s.cs, "% x");
    assert(s.rc < 0);
    s.rc = convspec_parse(&s.cs, "%+x");
    assert(s.rc < 0);
    /* The 'x' specifier does not support '-' and '0' simultaneously. */
    s.rc = convspec_parse(&s.cs, "%-0x");
    assert(s.rc < 0);
    /* The 'x' specifier supports all length modifiers but 'L'. */
    s.rc = convspec_parse(&s.cs, "%hhx");
    assert(s.rc == strlen("%hhx"));
    assert(s.cs.has_len == 1);
    assert(s.cs.len == CONVSPEC_CHAR);
    s.rc = convspec_parse(&s.cs, "%hx");
    assert(s.rc == strlen("%hx"));
    assert(s.cs.has_len == 1);
    assert(s.cs.len == CONVSPEC_SHORT);
    s.rc = convspec_parse(&s.cs, "%x");
    assert(s.rc == strlen("%x"));
    assert(s.cs.has_len == 0);
    assert(s.cs.len == 0);
    s.rc = convspec_parse(&s.cs, "%lx");
    assert(s.rc == strlen("%lx"));
    assert(s.cs.has_len == 1);
    assert(s.cs.len == CONVSPEC_LONG);
    s.rc = convspec_parse(&s.cs, "%llx");
    assert(s.rc == strlen("%llx"));
    assert(s.cs.has_len == 1);
    assert(s.cs.len == CONVSPEC_LONG_LONG);
    s.rc = convspec_parse(&s.cs, "%jx");
    assert(s.rc == strlen("%jx"));
    assert(s.cs.has_len == 1);
    assert(s.cs.len == CONVSPEC_MAX);
    s.rc = convspec_parse(&s.cs, "%tx");
    assert(s.rc == strlen("%tx"));
    assert(s.cs.has_len == 1);
    assert(s.cs.len == CONVSPEC_PTRDIFF);
    s.rc = convspec_parse(&s.cs, "%zx");
    assert(s.rc == strlen("%zx"));
    assert(s.cs.has_len == 1);
    assert(s.cs.len == CONVSPEC_SIZE);
    s.rc = convspec_parse(&s.cs, "%Lx");
    assert(s.rc < 0);
}

int main(int argc, char **argv)
{
    test_parse_no_spec();

    test_parse_simple();
    test_parse_length_modifier();
    test_parse_width();
    test_parse_argno();

    test_parse_full_percent();
    test_parse_full_char();
    test_parse_full_string();
    test_parse_full_signed();
    test_parse_full_unsigned();
    test_parse_full_hex();

    return 0;
}
