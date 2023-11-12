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
