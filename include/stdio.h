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

#include <stdarg.h> /* va_list */
#include <stddef.h> /* size_t, NULL */
#include <sys/types.h> /* ssize_t */

/* End-of-file return value. */
#define EOF ((int)-1)

/**
 * @brief      Puts a string on the standard output stream.
 *
 * @param[in]  val   Byte to be written to the standard output.
 *
 * @return     On success, the value written to the standard output.
 *             Otherwise, a negative value indicating the error code.
 */
int putchar(int val);

/**
 * @brief      Puts a string on the standard output stream.
 *
 * @param[in]  str   Pointer to the first character of the string to be written.
 *
 * @return     On success, 0 is returned. Otherwise, the value of @ref errno is
 *             set and @ref EOF is returned.
 */
int puts(const char *str);

/**
 * @brief      Write a formatted string to the standard output stream.
 *
 * @param[in]  fmt   String to be used to format the output.
 *
 * @return     On success, the number of characters written is returned.
 *             Otherwise, a negative value is returned and the value of
 *             @ref errno is set to reflect the error.
 */
int printf(const char *restrict fmt, ...);
int snprintf(char *restrict dest, size_t n, const char *restrict fmt, ...);
int sprintf(char *restrict dest, const char *restrict fmt, ...);

/**
 * @brief      Write a formatted string to the standard output stream.
 *
 * @param[in]  fmt   String to be used to format the output.
 * @param[in]  args  Argument list used for formatting.
 *
 * @return     On success, the number of characters written is returned.
 *             Otherwise, a negative value is returned and the value of
 *             @ref errno is set to reflect the error.
 */
int vprintf(const char *restrict fmt, va_list args);

int vsnprintf(char *restrict dest, size_t n, const char *restrict fmt, va_list args);
int vsprintf(char *restrict dest, const char *restrict fmt, va_list args);
