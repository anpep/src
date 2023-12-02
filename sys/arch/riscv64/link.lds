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
 
ENTRY(_start);

. = 0x80000000;

SECTIONS {
    .text : ALIGN(4K) {
        PROVIDE(__text_start = .);
        *(.text._start);
        *(.text*);
        PROVIDE(__text_end = .);
    }
    PROVIDE(__global_pointer = .);
    .rodata : ALIGN(4K) {
        PROVIDE(__rodata_start = .);
        *(.rodata*);
        PROVIDE(__rodata_end = .);
    }
    .data : ALIGN(4K) {
        PROVIDE(__data_start = .);
        *(.data*);
        PROVIDE(__data_end = .);
    }
    .bss : ALIGN(4K) {
        PROVIDE(__bss_start = .);
        *(.bss*);
        PROVIDE(__bss_end = .);
    }
    PROVIDE(__stack_start = .);
    PROVIDE(__stack_end = __stack_start + 0x80000);
    PROVIDE(__heap_start = .);
}
