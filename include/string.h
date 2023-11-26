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

#include <stddef.h> /* NULL, size_t */

/**
 * @brief      Obtains the length of the null-terminated string @p str.
 *
 * @param[in]  str   The null-terminated string.
 *
 * @return     The number of characters in @p str up to the null terminator.
 */
size_t strlen(const char *str);

/**
 * @brief      Compare the null-terminated strings @p str1 and @p str2
 *             lexicographically.
 *
 * @param[in]  str1  The first string to be compared.
 * @param[in]  str2  The second string to be compared.
 *
 * @return     A value greater than zero, equal to zero or less than zero;
 *             depending on @p str1 being greater than, equal to, or less than
 *             @p str2.
 */
int strcmp(const char *str1, const char *str2);

/**
 * @brief      Append a copy of the null-terminated string @p src to the end
 *             of the null-terminated string @p dst, then add a terminating
 *             '\0'.
 *
 * @param[out] dst   The destination null-terminated string.
 * @param[in]  src   The source null-terminated string.
 *
 * @return     The pointer @p dst.
 */
char *strcat(char *restrict dst, const char *restrict src);

/**
 * @brief      Append a copy of the null-terminated string @p src to the end
 *             of the null-terminated string @p dst, then add a terminating
 *             '\0'.
 *
 * @param[out] dst   The destination null-terminated string.
 * @param[in]  src   The source null-terminated string.
 * @param[in]  n     The maximum number of characters from @p src to be copied.
 *
 * @return     The pointer @p dst.
 */
char *strncat(char *restrict dst, const char *restrict src, size_t n);

/**
 * @brief      Writes @p len bytes of the value @p v to the @p dst buffer.
 *
 * @param      dst   The destination buffer.
 * @param[in]  v     The value to be written, converted to an unsigned char.
 * @param[in]  len   The times @p v will be written to @p dst.
 *
 * @return     The pointer @p dst.
 */
void *memset(void *dst, int v, size_t len);
