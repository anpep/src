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

#include <stdio.h>
#include <string.h>
#include <sys/arch/riscv64/csr.h>
#include <sys/panic.h>

extern void *__bss_start;
extern void *__bss_end;

void main(void);
void _trap(void);

/* TODO: call kernel console devices when we have that (: */
ssize_t write(int fd, const void *buf, size_t count)
{
    uint8_t *uart = (uint8_t *)0x10000000;
    for (size_t i = 0; i < count; i++) {
        *uart = ((uint8_t *)buf)[i];
    }
    return (ssize_t)count;
}

void
#ifndef __clang__
    __attribute__((naked, section(".start")))
#endif
    _start(void)
{
    /* Set global and stack pointers, and call _early_init(). */
    __asm__(".option push; .option norelax;"
            "la gp, __global_pointer;"
            ".option pop;"
            "la sp, __stack_top;"
            "jal zero, _early_init");
}

void __attribute__((noreturn)) _end(int exit_code)
{
    for (;;) {
        __asm__("csrw mie, zero; wfi");
    }
}

void _init(void)
{
    /* _init() runs in M-mode. */
    /* Clear .bss section. */
    memset(&__bss_start, 0, (size_t)(&__bss_end - &__bss_start));
    /* Setup trap handler. */
    csr_write(CSR_MTVEC, (uintptr_t)&_trap);
    main();
}

void _early_init(void)
{
    /* Reset supervisor access translation and protection register. */
    csr_write(CSR_SATP, 0);
    /* Set privilege mode. */
    csr_mstatus_t s;
    s.value = csr_read(CSR_MSTATUS);
    s.fields.mpp = PRIV_MACHINE;
    csr_write(CSR_MSTATUS, s.value);
    /* Set machine exception program counter. This makes the `mret` instruction
       jump to _init() in M-mode. */
    csr_write(CSR_MEPC, (uintptr_t)&_init);
    /* Setting return address to _end() will halt the machine after _init()
       returns. */
    __asm__("la ra, _end");
    /* Jump to _init() in M-mode. */
    __asm__("mret");
}
