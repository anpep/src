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

#include <string.h>

size_t strlen(const char *str)
{
    size_t len = 0;
    while (*str++) {
        len++;
    }
    return len;
}

int strcmp(const char *str1, const char *str2)
{
    while (*str1 != '\0' && *str1 == *str2) {
        ++str1, ++str2;
    }
    return *str1 - *str2;
}

char *strcat(char *restrict dst, const char *restrict src)
{
    char *dst_orig = dst;
    while (*dst++) { }
    while ((*dst++ = *src++) != '\0') { }
    return dst_orig;
}

char *strncat(char *restrict dst, const char *restrict src, size_t n)
{
    char *dst_orig = dst;
    while (*dst++) { }
    while (n-- > 0 && (*dst++ = *src++) != '\0') { }
    return dst_orig;
}

void *memset(void *dst, int v, size_t len)
{
    unsigned char *buf = ((unsigned char *)dst);
    for (size_t i = 0; i < len; i++) {
        buf[i] = (unsigned char)v & 0xFFU;
    }
    return dst;
}
