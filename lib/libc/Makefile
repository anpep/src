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

CLANG_FORMAT ?= clang-format
CLANG_TIDY ?= clang-tidy
CC ?= cc
AR ?= ar

CLANG_FORMAT_FLAGS ?= -style file -Werror
COMMON_CFLAGS := \
	-std=c99 \
	-ffreestanding -fno-builtin -nostdlib -nostdinc \
	-Wall -Wextra -Wpedantic -pedantic -Wno-unused-variable -Wno-unused-parameter \
	-I../../include

HEADERS := $(wildcard */*.h *.h)
SOURCES := $(wildcard */*.S *.S) $(wildcard */*.c *.c)
OBJECTS := $(patsubst %.S,%.o,$(patsubst %.c,%.o,${SOURCES}))

ARCH_PATH := ../../sys/arch/${ARCH}
-include ${ARCH_PATH}/Makefile

CFLAGS := ${ARCH_CFLAGS} ${COMMON_CFLAGS}

export TARGET := ../libc.a

all: ${TARGET}

${TARGET}: ${OBJECTS}
	${AR} rcs $@ $^

clean:
	${RM} ${TARGET} ${OBJECTS}

lint:
	${CLANG_FORMAT} ${CLANG_FORMAT_FLAGS} --dry-run ${HEADERS} ${SOURCES}||exit 1
	${CLANG_TIDY} ${CLANG_TIDY_FLAGS} ${HEADERS} ${SOURCES} -- ${COMMON_CFLAGS}||exit 1

.PHONY: all clean lint
