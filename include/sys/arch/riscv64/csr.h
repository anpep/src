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

#include <stdint.h>

enum csr {
    CSR_SSTATUS = 0x100, /* Supervisor status register. */
    CSR_STVEC = 0x105, /* Supervisor trap-handler base address. */
    CSR_SSCRATCH = 0x140, /* Scratch register for supervisor trap handlers. */
    CSR_SEPC = 0x141, /* Supervisor exception program counter. */
    CSR_SCAUSE = 0x142, /* Supervisor trap cause. */
    CSR_STVAL = 0x143, /* Supervisor bad address or instruction. */
    CSR_SATP = 0x180, /* Supervisor address translation and protection. */

    CSR_MHARTID = 0xF14, /* Hardware thread ID. */
    CSR_MSTATUS = 0x300, /* Machine status register. */
    CSR_MTVEC = 0x305, /* Machine trap-handler base address. */
    CSR_MEPC = 0x341, /* Machine exception program counter. */
    CSR_MCAUSE = 0x342, /* Machine trap cause. */
    CSR_MTVAL = 0x343, /* Machine bad address or instruction. */
};

/**
 * @brief      Reads the value of the specified Control/Status Register.
 *
 * @param[in]  num   The CSR number.
 *
 * @return     The value of the CSR.
 */
inline uint64_t __attribute__((always_inline)) csr_read(const enum csr num)
{
    uint64_t ret;
    __asm__("csrr %0, %1" : "=r"(ret) : "i"(num));
    return ret;
}

/**
 * @brief      Writes to a specified Control/Status Register.
 *
 * @param[in]  num   The CSR number.
 * @param[in]  val   The value to be written.
 */
inline void __attribute__((always_inline))
csr_write(const enum csr num, uint64_t val)
{
    __asm__("csrw %0, %1" ::"i"(num), "r"(val));
}

/* Supervisor-mode status register. */
typedef union {
    struct {
        unsigned _wpri0 : 1;
        unsigned sie : 1;
        unsigned _wpri1 : 1;
        unsigned mie : 1;
        unsigned _wpri2 : 1;
        unsigned spie : 1;
        unsigned ube : 1;
        unsigned mpie : 1;
        enum {
            PRIV_USER = 0, /*U-level privilege. */
            PRIV_SUPERVISOR = 1, /* S-level privilege. */
        } spp : 1; /* S-level privilege mode. */
        unsigned vs : 2;
        enum {
            PRIV_MACHINE = 3, /* M-level privilege. */
        } mpp : 2; /* M-level privilege mode. */
        unsigned fs : 2;
        unsigned xs : 2;
        unsigned mprv : 1;
        unsigned sum : 1;
        unsigned mxr : 1;
        unsigned tvm : 1;
        unsigned tw : 1;
        unsigned tsr : 1;
        unsigned _wpri3 : 9;
        unsigned uxl : 2;
        unsigned sxl : 2;
        unsigned sbe : 1;
        unsigned mbe : 1;
        unsigned _wpri4 : 25;
        unsigned sd : 1;
    } fields;
    uint64_t value;
} csr_mstatus_t;

/* Supervisor address translation and protection register. */
typedef union {
    struct {
        unsigned long long ppn : 44; /* Physical page number. */
        unsigned asid : 16; /* Address space identifier. */
        enum {
            SATP_MODE_BARE = 0, /* No translation or protection. */
            SATP_MODE_SV39 = 8, /* Page-based 39-bit virtual addressing. */
            SATP_MODE_SV48 = 9, /* Page-based 48-bit virtual addressing. */
            SATP_MODE_SV57 = 10, /* Page-based 57-bit virtual addressing. */
        } mode : 4;
    } fields;
    uint64_t value;
} csr_satp_t;

/* PMP configuration register. */
typedef union {
    struct {
        unsigned r : 1; /* Read. */
        unsigned w : 1; /* Write. */
        unsigned x : 1; /* Execute. */
        enum {
            ADDRMATCH_OFF = 0, /* Null region (disabled). */
            ADDRMATCH_TOR = 1, /* Top of range. */
            ADDRMATCH_NA4 = 2, /* Naturally aligned four-byte region. */
            /* Naturally aligned power-of-two region, >= 8 bytes). */
            ADDRMATCH_NAPOT = 3,
        } a : 2; /* Address matching mode. */
        unsigned zero : 2;
        unsigned l : 1; /* Entry lock bit. */
    } fields;
    uint8_t value;
} csr_pmpxcfg_t;

typedef union {
    csr_pmpxcfg_t regs[8];
    uint64_t value;
} csr_pmpcfgx_t;
