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

#include "priv.h"

enum csr {
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

/* Machine status register. */
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
        enum spriv spp : 1; /* S-level privilege mode. */
        unsigned vs : 2;
        enum mpriv mpp : 2; /* M-level privilege mode. */
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
