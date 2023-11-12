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
#include <stdlib.h>

long int strtol(const char* restrict nptr, char** restrict endptr, int base)
{
    errno = ENOSYS;
    return -1;
}

long long int strtoll(
    const char* restrict nptr, char** restrict endptr, int base)
{
    errno = ENOSYS;
    return -1;
}

unsigned long int strtoul(
    const char* restrict nptr, char** restrict endptr, int base)
{
    errno = ENOSYS;
    return ~0UL;
}

unsigned long long int strtoull(
    const char* restrict nptr, char** restrict endptr, int base)
{
    errno = ENOSYS;
    return ~0ULL;
}
