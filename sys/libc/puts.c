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

int putchar(int val)
{
#ifdef _KERNEL
    /* TODO: Invoke kernel console driver. */
    extern int early_putchar(int);
    early_putchar(val);
    return val;
#else /* !_KERNEL */
    return -ENOSYS;
#endif /* _KERNEL */
}

int puts(const char *str)
{
    int retval;

    while (*str++) {
        retval = putchar(*str);
        if (retval < 0) {
            errno = retval;
            return EOF;
        }
    }

    return 0;
}
