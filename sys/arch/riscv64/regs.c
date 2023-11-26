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

#include <stdint.h>
#include <stdio.h>
#include <sys/arch/riscv64/regs.h>

int regs_print(const struct regs *r)
{
    const uint64_t *rb = (const uint64_t *)r;
    /* Temporary registers t0-t6. */
    for (int t = 0; t <= 6; t++, rb++) {
        printf("t%-3d=0x%08x\t", t, *rb);
        if (t % 3 > 1 || t == 6) {
            puts("\n");
        }
    }
    /* Saved registers s0-s11. */
    for (int s = 0; s <= 11; s++, rb++) {
        printf("s%-3d=0x%08x\t", s, *rb);
        if (s % 3 > 1 || s == 11) {
            puts("\n");
        }
    }
    /* Function argument registers a0-a7. */
    for (int a = 0; a <= 7; a++, rb++) {
        printf("a%-3d=0x%08x\t", a, *rb);
        if (a % 3 > 1 || a == 7) {
            puts("\n");
        }
    }
    /* Special registers. */
    printf("zero=0x%08x\t", r->zero);
    printf("ra  =0x%08x\t", r->ra);
    printf("sp  =0x%08x\t\n", r->sp);
    printf("gp  =0x%08x\t", r->gp);
    printf("tp  =0x%08x\t\n", r->tp);
    return 0;
}
