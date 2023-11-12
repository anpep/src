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
 * @brief      Test whether the given character is a decimal digit.
 *
 * @param[in]  chr   The character to test.
 *
 * @return     1 if the character is a digit, otherwise 0.
 */
inline __attribute__((always_inline)) int isdigit(int chr)
{
    return chr >= '0' && chr <= '9';
}

/**
 * @brief      Test whether the given character is a space.
 *
 * @param[in]  chr   The character to test.
 *
 * @return     1 if the character is a space, otherwise 0.
 */
inline __attribute__((always_inline)) int isspace(int chr)
{
    return chr == ' ' || chr == '\t' || chr == '\v' || chr == '\n';
}

/**
 * @brief      Test whether the given character is a lowercase letter.
 *
 * @param[in]  chr   The character to test.
 *
 * @return     1 if the character is lowercase, otherwise 0.
 */
inline __attribute__((always_inline)) int islower(int chr)
{
    return chr >= 'a' && chr <= 'z';
}

/**
 * @brief      Test whether the given character is an uppercase letter.
 *
 * @param[in]  chr   The character to test.
 *
 * @return     1 if the character is uppercase, otherwise 0.
 */
inline __attribute__((always_inline)) int isupper(int chr)
{
    return chr >= 'A' && chr <= 'Z';
}

/**
 * @brief      Convert a character to lowercase.
 *
 * @param[in]  chr   The character to be converted.
 *
 * @return     The converted character.
 */
inline __attribute__((always_inline)) int tolower(int chr)
{
    return isupper(chr) ? (chr + ('a' - 'A')) : chr;
}

/**
 * @brief      Convert a character to lowercase.
 *
 * @param[in]  chr   The character to be converted.
 *
 * @return     The converted character.
 */
inline __attribute__((always_inline)) int toupper(int chr)
{
    return islower(chr) ? (chr + ('A' - 'a')) : chr;
}
