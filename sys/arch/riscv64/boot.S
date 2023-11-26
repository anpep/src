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

/* Disable compressed instructions. */
.option norvc

.type start, @function
.global start
start:
    .cfi_startproc

.option push
.option norelax
    /* We don't want linker relaxation to happen here because relaxation emits
     * shorter instructions with addresses relative to the global pointer.
     * Since the GP is still uninitialized, this behavior is undesirable! */
    la gp, global_pointer
.option pop

    /* Reset SATP. */
    csrw satp, zero

    /* Set up the stack. */
    la sp, stack_top

    /* Clear the BSS section .*/
    la t5, bss_start
    la t6, bss_end
_bss_clear:
    sd zero, (t5)
    addi t5, t5, 8
    bltu t5, t6, _bss_clear

    /* XXX: What does this do? */
    /*la t0, kmain
    csrw mepc, t0*/
    tail main

    .cfi_endproc
.end
