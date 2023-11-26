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
#include <stdio.h>
#include <sys/panic.h>

ssize_t write(int fd, const void *buf, size_t count)
{
    uint8_t *uart = (uint8_t *)0x10000000;
    for (size_t i = 0; i < count; i++) {
        *uart = ((uint8_t *)buf)[i];
    }
    return (ssize_t)count;
}

void init(void) { panic("init() has run"); }
