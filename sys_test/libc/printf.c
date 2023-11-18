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

#define printf uut_printf
#define vprintf uut_vprintf
#include "../../sys/libc/printf.c"
#undef printf
#undef vprintf

/* Test state. */
struct state {
    size_t write_calls;
    char write_buf[1024];
    size_t write_len;
} g_state;

static void prepare_test(struct state *state)
{
    memset(state, 0, sizeof(*state));
}

ssize_t write(int fd, const void *buf, size_t count)
{
    printf("write(%d, \"%s\", %d)\n", fd, buf, count);
    g_state.write_calls++;
    strncat(g_state.write_buf, buf, count);
    g_state.write_len += count;
    return (ssize_t)count;
}

static void test_vfprintf_impl(void)
{
    int rc;

    /* Test writing strings with no conversion specifier. */
    prepare_test(&g_state);
    rc = uut_printf("hello");
    assert(rc == strlen("hello"));
    assert(!strcmp(g_state.write_buf, "hello"));
    assert(g_state.write_len == strlen("hello"));
    assert(g_state.write_calls == 1);

    /* Test writing simple characters. */
    prepare_test(&g_state);
    rc = uut_printf("hello%cworld", ' ');
    assert(rc == strlen("hello world"));
    assert(!strcmp(g_state.write_buf, "hello world"));
    assert(g_state.write_len == strlen("hello world"));
    assert(g_state.write_calls == 3);

    /* Test writing simple strings. */
    prepare_test(&g_state);
    rc = uut_printf("hello%sworld%s", ", ", "!\n");
    assert(rc == strlen("hello, world!\n"));
    assert(!strcmp(g_state.write_buf, "hello, world!\n"));
    assert(g_state.write_len == strlen("hello, world!\n"));
    assert(g_state.write_calls == 4);

    /* Test leading padding. */
    prepare_test(&g_state);
    rc = uut_printf("%4c", 'x');
    assert(rc == 4);
    assert(!strcmp(g_state.write_buf, "   x"));
    assert(g_state.write_len == 4);
    assert(g_state.write_calls >= 2);

    /* Test trailing padding. */
    prepare_test(&g_state);
    rc = uut_printf("%-4c", 'x');
    assert(rc == 4);
    assert(!strcmp(g_state.write_buf, "x   "));
    assert(g_state.write_len == 4);
    assert(g_state.write_calls >= 2);

    /* Test precision part on strings. */
    prepare_test(&g_state);
    rc = uut_printf("%.5s", "hello world");
    assert(rc == 5);
    assert(!strcmp(g_state.write_buf, "hello"));
    assert(g_state.write_len == 5);
    assert(g_state.write_calls == 1);

    /* Test signed integers. */
    prepare_test(&g_state);
    rc = uut_printf("%d", 2147483647);
    assert(rc == 10);
    assert(!strcmp(g_state.write_buf, "2147483647"));
    assert(g_state.write_len == 10);
    prepare_test(&g_state);
    rc = uut_printf("%+d", 2147483647);
    assert(rc == 11);
    assert(!strcmp(g_state.write_buf, "+2147483647"));
    assert(g_state.write_len == 11);
    prepare_test(&g_state);
    rc = uut_printf("% d", -2147483647);
    assert(rc == 11);
    assert(!strcmp(g_state.write_buf, "-2147483647"));
    assert(g_state.write_len == 11);
    prepare_test(&g_state);
    rc = uut_printf("%4d", -42);
    assert(rc == 4);
    assert(!strcmp(g_state.write_buf, " -42"));
    assert(g_state.write_len == 4);
    prepare_test(&g_state);

    /* Test unsigned integers. */
    prepare_test(&g_state);
    rc = uut_printf("%u", 2147483647);
    assert(rc == 10);
    assert(!strcmp(g_state.write_buf, "2147483647"));
    assert(g_state.write_len == 10);
    prepare_test(&g_state);
    rc = uut_printf("%4u", 42);
    assert(rc == 4);
    assert(!strcmp(g_state.write_buf, "  42"));
    assert(g_state.write_len == 4);
    prepare_test(&g_state);
}

int main(int argc, char **argv)
{
    test_vfprintf_impl();
    return 0;
}
