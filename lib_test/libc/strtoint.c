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

#define strtol __strtol
#define strtoll __strtoll
#define strtoul __strtoul
#define strtoull _strtoull

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#undef strtol
#undef strtoll
#undef strtoul
#undef strtoull

#include "../../lib/libc/strtoint.c"

static void test_strtol(void)
{
    char *input;
    char *endptr;
    long rc;

    /* Test decimal guess with padding characters. */
    errno = 0;
    input = "    \t\v42nice";
    rc = strtol(input, &endptr, 0);
    assert(rc == 42);
    assert(errno == 0);
    assert(!strcmp(endptr, "nice"));

    /* Test hex string. */
    errno = 0;
    input = "cd0";
    rc = strtol(input, &endptr, 10);
    assert(rc == 0);
    assert(!strcmp(endptr, "cd0"));

    /* Test octal guess. */
    errno = 0;
    input = "0777";
    rc = strtol(input, &endptr, 0);
    assert(rc == 0777);
    assert(errno == 0);
    assert(!strcmp(endptr, ""));

    /* Test hexadecimal guess. */
    errno = 0;
    input = "0xfFfFfFfF";
    rc = strtol(input, &endptr, 0);
    assert(rc == 0xFFFFFFFF);
    assert(errno == 0);
    assert(!strcmp(endptr, ""));

    /* Test explicit binary with no endptr. */
    errno = 0;
    input = "101001";
    rc = strtol(input, NULL, 2);
    assert(rc == 41);
    assert(errno == 0);

    /* Test explicit base-36 with no endptr. */
    errno = 0;
    input = "zIK0zj";
    rc = strtol(input, NULL, 36);
    assert(rc == INT32_MAX);
    assert(errno == 0);

    /* Test explicit negative. */
    errno = 0;
    input = "-42";
    rc = strtol(input, NULL, 10);
    assert(rc == -42);
    assert(errno == 0);

    /* Test explicit positive. */
    errno = 0;
    input = "+00000002";
    rc = strtol(input, NULL, 4);
    assert(rc == 2);
    assert(errno == 0);

    /* Test bounds. */
    errno = 0;
    input = "-922337203685477580721234";
    rc = strtol(input, &endptr, 10);
    assert(rc == LONG_MIN);
    assert(!strcmp(endptr, "21234"));
    assert(errno == ERANGE);
}

static void test_strtoul(void)
{
    char *input;
    char *endptr;
    unsigned long rc;

    /* Test decimal guess with padding characters. */
    errno = 0;
    input = "-1";
    rc = strtoul(input, &endptr, 0);
    assert(rc == 0);
    assert(errno == EINVAL);
    assert(!strcmp(endptr, "-1"));
}

int main(int argc, char **argv)
{
    test_strtol();
    test_strtoul();
    return 0;
}
