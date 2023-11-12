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

/**
 * @brief      Convert the given string to a long int value.
 *
 * @param[in]  nptr    The string to be converted.
 * @param      endptr  If not NULL, a pointer to a string starting immediately
 *                     after the last valid digit of a numerical character
 *                     sequence.
 * @param[in]  base    The base (between 2 and 36, including both) to be used
 *                     when converting the string. If 0, the base will be
 *                     guessed accepting the "0x" and "0X" prefixes for
 *                     indicating a base of 16, and the "0" prefix for
 *                     indicating a base of 8.
 *
 * @return     The parsed numeric value. If no conversion could be made,
 *             0 is returned and errno is set accordingly.
 */
long int strtol(const char *restrict nptr, char **restrict endptr, int base);

/**
 * @brief      Convert the given string to a long long int value.
 *
 * @param[in]  nptr    The string to be converted.
 * @param      endptr  If not NULL, a pointer to a string starting immediately
 *                     after the last valid digit of a numerical character
 *                     sequence.
 * @param[in]  base    The base (between 2 and 36, including both) to be used
 *                     when converting the string. If 0, the base will be
 *                     guessed accepting the "0x" and "0X" prefixes for
 *                     indicating a base of 16, and the "0" prefix for
 *                     indicating a base of 8.
 *
 * @return     The parsed numeric value. If no conversion could be made,
 *             0 is returned and errno is set accordingly.
 */
long long int strtoll(
    const char *restrict nptr, char **restrict endptr, int base);

/**
 * @brief      Convert the given string to an unsigned long int value.
 *
 * @param[in]  nptr    The string to be converted.
 * @param      endptr  If not NULL, a pointer to a string starting immediately
 *                     after the last valid digit of a numerical character
 *                     sequence.
 * @param[in]  base    The base (between 2 and 36, including both) to be used
 *                     when converting the string. If 0, the base will be
 *                     guessed accepting the "0x" and "0X" prefixes for
 *                     indicating a base of 16, and the "0" prefix for
 *                     indicating a base of 8.
 *
 * @return     The parsed numeric value. If no conversion could be made,
 *             0 is returned and errno is set accordingly.
 */
unsigned long int strtoul(
    const char *restrict nptr, char **restrict endptr, int base);

/**
 * @brief      Convert the given string to an unsigned long long int value.
 *
 * @param[in]  nptr    The string to be converted.
 * @param      endptr  If not NULL, a pointer to a string starting immediately
 *                     after the last valid digit of a numerical character
 *                     sequence.
 * @param[in]  base    The base (between 2 and 36, including both) to be used
 *                     when converting the string. If 0, the base will be
 *                     guessed accepting the "0x" and "0X" prefixes for
 *                     indicating a base of 16, and the "0" prefix for
 *                     indicating a base of 8.
 *
 * @return     The parsed numeric value. If no conversion could be made,
 *             0 is returned and errno is set accordingly.
 */
unsigned long long int strtoull(
    const char *restrict nptr, char **restrict endptr, int base);
