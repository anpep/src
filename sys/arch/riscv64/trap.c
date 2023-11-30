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

#include "stddef.h"
#include <stdint.h>
#include <stdio.h>
#include <sys/arch/riscv64/csr.h>
#include <sys/panic.h>

static const char *exception_descs[] = {
    [0] = "Instruction address misaligned",
    [1] = "Instruction access fault",
    [2] = "Illegal instruction",
    [3] = "Breakpoint",
    [4] = "Load address misaligned",
    [5] = "Load access fault",
    [6] = "Store/AMO address misaligned",
    [7] = "Store/AMO access fault",
    [8] = "Environment call from U-mode",
    [9] = "Environment call from S-mode",
    [10] = NULL,
    [11] = "Environment call from M-mode",
    [12] = "Instruction page fault",
    [13] = "Load page fault",
    [14] = NULL,
    [15] = "Store/AMO page fault",
};

void __attribute__((noinline)) _trap(void)
{
    const uint64_t CAUSE_INT_MASK = 0x80000000;
    const uint64_t CAUSE_CODE_MASK = 0x7fffffff;

    uint64_t cause = csr_read(CSR_MCAUSE);
    uint64_t cause_code = (cause & CAUSE_CODE_MASK);

    if ((cause & CAUSE_INT_MASK) != 0) {
        /* Interrupt. */
        printf("int: cause_code=0x%016llx\n", cause_code);
    } else {
        /* Exception. */
        uint64_t hartid = csr_read(CSR_MHARTID);
        uint64_t epc = csr_read(CSR_MEPC);
        uint64_t tval = csr_read(CSR_MTVAL);

        size_t descs_len = sizeof(exception_descs) / sizeof(*exception_descs);
        char *desc = "unknown";
        if (cause_code < descs_len && exception_descs[cause_code] != NULL) {
            desc = (char *)exception_descs[cause_code];
        }
        panic("exception on hart %d at 0x%llx: %s "
              "(cause=%lld, tval=0x%llx)",
            hartid, epc, desc, cause_code, tval);
    }
}
