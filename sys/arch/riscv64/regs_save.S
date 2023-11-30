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
 
.section .text

.type regs_save, @function
.global regs_save
regs_save:
    .cfi_startproc
    sd t0, (0 * 8)(a0)
    sd t1, (1 * 8)(a0)
    sd t2, (2 * 8)(a0)
    sd t3, (3 * 8)(a0)
    sd t4, (4 * 8)(a0)
    sd t5, (5 * 8)(a0)
    sd t6, (6 * 8)(a0)

	sd s0, (7 * 8)(a0)
	sd s1, (8 * 8)(a0)
	sd s2, (9 * 8)(a0)
	sd s3, (10 * 8)(a0)
	sd s4, (11 * 8)(a0)
	sd s5, (12 * 8)(a0)
	sd s6, (13 * 8)(a0)
	sd s7, (14 * 8)(a0)
	sd s8, (15 * 8)(a0)
	sd s9, (16 * 8)(a0)
	sd s10, (17 * 8)(a0)
	sd s11, (18 * 8)(a0)
    
    sd a0, (19 * 8)(a0)
    sd a1, (20 * 8)(a0)
    sd a2, (21 * 8)(a0)
    sd a3, (22 * 8)(a0)
    sd a4, (23 * 8)(a0)
    sd a5, (24 * 8)(a0)
    sd a6, (25 * 8)(a0)
    sd a7, (26 * 8)(a0)

    sd zero, (27 * 8)(a0)
    sd ra, (28 * 8)(a0)
    sd sp, (29 * 8)(a0)
    sd gp, (30 * 8)(a0)
    sd tp, (31 * 8)(a0)

	/* Return zero */
	li a0, 0
    ret
    .cfi_endproc
.end
