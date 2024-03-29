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
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int putchar(int val)
{
    ssize_t rc;
    rc = write(0, &val, 1);
    if (rc < 0) {
        return (int)rc;
    }
    return 0;
}

int puts(const char *str)
{
    ssize_t rc;
    rc = write(0, str, strlen(str));
    if (rc < 0) {
        errno = (int)rc;
        return EOF;
    }
    return 0;
}
