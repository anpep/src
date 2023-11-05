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

#pragma once

/* Numerical limits. */

#define CHAR_BIT (8 * sizeof(char))
#define CHAR_MAX UCHAR_MAX
#define CHAR_MIN 0
#define SCHAR_MAX 0x7f
#define SCHAR_MIN (-0x7f - 1)
#define UCHAR_MAX 0xff

#define SHRT_MAX 0x7fff
#define SHRT_MIN (-0x7fff - 1)
#define USHRT_MAX 0xffff

#define WORD_BIT 32
#define INT_MAX 0x7fffffff
#define INT_MIN (-0x7fffffff - 1)
#define UINT_MAX 0xffffffff

#define LONG_BIT 64
#define LONG_MAX 0x7fffffffffffffff
#define LONG_MIN (-0x7fffffffffffffff - 1)
#define ULONG_MAX 0xffffffffffffffff

#define LLONG_BIT 64
#define LLONG_MAX 0x7fffffffffffffff
#define LLONG_MIN (-0x7fffffffffffffff - 1)
#define ULLONG_MAX 0xffffffffffffffff
