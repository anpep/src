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
CLANG_TIDY_FLAGS ?= -header-filter=.*
CFLAGS := \
	-std=c99 -fprofile-arcs -ftest-coverage \
	-Wall -Wextra -Wpedantic -Wno-unused-variable -Wno-unused-parameter \
	-D_TEST

HEADERS := $(wildcard */*.h *.h)
SOURCES := $(wildcard */*.c *.c)
BINARIES := $(patsubst %.c,%,${SOURCES})

all: test

test: ${BINARIES}
	for test in $^ ; do ./$$test||exit 1; done

clean:
	${RM} ${BINARIES} *.gcno *.gcda *.gcov

lint:
	${CLANG_FORMAT} ${CLANG_FORMAT_FLAGS} --dry-run ${HEADERS} ${SOURCES}
	${CLANG_TIDY} ${CLANG_TIDY_FLAGS} ${HEADERS} ${SOURCES} -- ${CFLAGS}

.PHONY: all clean lint
