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

CLANG_FORMAT_FLAGS ?= -style file -Werror
CFLAGS := \
	-std=c99 -fprofile-arcs -ftest-coverage \
	-fsanitize=undefined -fno-sanitize=signed-integer-overflow \
	-Wall -Wextra -Wpedantic -pedantic -Wno-unused-variable -Wno-unused-parameter -Wno-sign-compare \
	-D_TEST

HEADERS := $(wildcard */*.h *.h)
SOURCES := $(wildcard */*.c *.c)
BINARIES := $(patsubst %.c,%,${SOURCES})

all: test

test: ${BINARIES}
	for test in $^ ; do ./$$test&&rm ./$$test||exit 1; done

clean:
	${RM} ${BINARIES} *.gcno *.gcda *.gcov

lint:
	${CLANG_FORMAT} ${CLANG_FORMAT_FLAGS} --dry-run ${HEADERS} ${SOURCES}||exit 1
	${CLANG_TIDY} ${CLANG_TIDY_FLAGS} ${HEADERS} ${SOURCES} -- ${CFLAGS}||exit 1

.PHONY: all clean lint
