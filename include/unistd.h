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

#include <stddef.h>
#include <sys/types.h>

/**
 * @brief      Write @ref count bytes from the buffer pointed to by @ref buf
 *             to the file associated with the open file descriptor @ref fd.
 *
 * @param[in]  fd     Open file descriptor of the destination stream.
 * @param[in]  buf    The buffer to be written to the destination stream.
 * @param[in]  count  The number of bytes to be written.
 *
 * @return     On success, the number of bytes written to the destination
 *             stream; otherwise, -1 will be returned and @ref errno will be
 *             set accordingly.
 */
ssize_t write(int fd, const void *buf, size_t count);
