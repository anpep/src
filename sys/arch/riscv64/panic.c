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

#include <stdarg.h>
#include <stdio.h>
#include <sys/arch/riscv64/regs.h>
#include <sys/panic.h>

void __attribute__((noreturn)) panic(const char *fmt, ...)
{
    struct regs r;
    va_list v;

    regs_save(&r);
    va_start(v, fmt);
    puts("panic: ");
    vprintf(fmt, v);
    putchar('\n');
    va_end(v);
    regs_print(&r);
    puts("system halted\n");
    for (;;) {
        __asm__("wfi");
    }
}
