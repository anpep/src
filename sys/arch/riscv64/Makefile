##
## Copyright (C) 2023 Ángel Pérez <ap@anpep.co>
##
## This program is free software: you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by the Free
## Software Foundation, either version 3 of the License, or (at your option)
## any later version.
##
## This program is distributed in the hope that it will be useful, but WITHOUT
## ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
## FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
## more details.
##
## You should have received a copy of the GNU General Public License along with
## this program.  If not, see <http://www.gnu.org/licenses/>.
##

export ARCH_PREFIX ?= riscv64-unknown-elf-
export CC := ${ARCH_PREFIX}gcc
export AS := ${ARCH_PREFIX}as
export LD := ${ARCH_PREFIX}gcc
export AR := ${ARCH_PREFIX}ar

export ARCH_CFLAGS := -ggdb -mcmodel=medany
export ARCH_LDFLAGS := -T${ARCH_PATH}/link.lds

export ARCH_HEADERS := $(wildcard ${ARCH_PATH}/*/*.h ${ARCH_PATH}/*.h)
export ARCH_SOURCES := $(wildcard ${ARCH_PATH}/*/*.S ${ARCH_PATH}/*.S) $(wildcard ${ARCH_PATH}/*/*.c ${ARCH_PATH}/*.c)
export ARCH_TARGET := boot.elf
