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

TARGETS := sys sys_test

.PHONY: mkcompdb all
all: mkcompdb ${TARGETS}
mkcompdb: FORCE
	@./tool/mkcompdb

.PHONY: clean $(addprefix clean_,${TARGETS})
clean: $(addprefix clean_,${TARGETS})

FORCE: ;

define target_template
${1}: FORCE
	@${MAKE} -C ${1}

clean_${1}: FORCE
	@${MAKE} -C ${1} clean
endef

$(foreach target,${TARGETS},$(eval $(call target_template,${target})))
