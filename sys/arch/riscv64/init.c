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
#include <sys/arch/riscv64/pt.h>
#include <sys/panic.h>

/* TODO: call kernel console devices when we have that (: */
ssize_t write(int fd, const void *buf, size_t count)
{
    uint8_t *uart = (uint8_t *)0x10000000;
    for (size_t i = 0; i < count; i++) {
        *uart = ((uint8_t *)buf)[i];
    }
    return (ssize_t)count;
}

void __attribute__((naked)) _start(void)
{
    /* Set global and stack pointers, and call _early_init(). */
    __asm__(".option push; .option norelax;"
            "la gp, __global_pointer;"
            ".option pop;"
            "la sp, __stack_end;"
            "jal zero, _early_init");
}

void __attribute__((noreturn)) _end(int exit_code)
{
    for (;;) {
        __asm__("wfi");
    }
}

void kmain(void);
void _trap(void);

void _early_init(void)
{
    /* Disable supervisor address translation and protection. */
    csr_write(CSR_SATP, 0);
    /* Setup trap handler. */
    csr_write(CSR_MTVEC, (uintptr_t)&_trap);
    /* Set machine privilege mode. */
    csr_mstatus_t s = { 0 };
    s.value = csr_read(CSR_MSTATUS);
    s.fields.mpp = PRIV_MACHINE;
    csr_write(CSR_MSTATUS, s.value);
    /* Set machine exception program counter. This makes the `mret`
       instruction jump to main() in M-mode. */
    csr_write(CSR_MEPC, (uintptr_t)&kmain);
    /* Setting return address to _end() will halt the machine after main()
       returns. */
    __asm__("la ra, _end");
    /* Jump to main() in M-mode. */
    __asm__("mret");
}
