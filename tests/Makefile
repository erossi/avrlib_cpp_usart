# Copyright (C) 2024 Enrico Rossi
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
# USA

# Overridable parameters
OPTLEV ?= 2
MCU ?= atmega1284p
FCPU ?= 1000000UL

PWD = $(shell pwd)
INCLUDE_DIR := $(PWD)/../include
LIB_DIR ?= $(PWD)/../lib

# Compilers
CXX := avr-g++
BUILD_DIR ?= /run/user/$(shell id --user)/build
CXXFLAGS ?= -I$(INCLUDE_DIR) \
	    -Wall \
	    -Wextra \
	    -pedantic \
	    -std=c++14 \
	    -mmcu=$(MCU) \
	    -O$(OPTLEV) \
	    -fshort-enums \
	    -D F_CPU=$(FCPU)
LFLAGS = -lm

# Uncomment if git is in use
LIBNAME ?= usart

RM := rm -f

.PHONY: clean indent
.SILENT: help
.SUFFIXES: .cpp, .o

# set V=1 (eg, "make V=1") to print the full commands etc.
ifneq ($V,1)
 Pecho=@echo
 P=@
else
 Pecho=@:
 P=
endif

tests: test_echo \
	test_echo_p2 \
	test_echo_rxcbuffer \
	test_echo_rxcbuffer_p2 \
	test_echo_x2

# Standard build
# WARN: positional argument are important
%.o: %.cpp
	$(Pecho) "  CXX   $<"
	$P $(CXX) -MMD -c $(CXXFLAGS) $< -o $(BUILD_DIR)/$@

test_%:
	$(Pecho) "  CXX   $@"
	$P $(CXX) $(CXXFLAGS) \
		$@.cpp \
		-L$(LIB_DIR) -l$(LIBNAME) $(LFLAGS) \
		-o $(BUILD_DIR)/$@.elf

# Specific compilations
test_echo_p2: $(common_o)
	$(Pecho) "  CXX   $@"
	$P $(CXX) $(CXXFLAGS) \
		-D _USE_USART1_ \
		test_echo.cpp \
		-L$(LIB_DIR) -l$(LIBNAME) $(LFLAGS) \
		-o $(BUILD_DIR)/$@.elf

test_echo_rxcbuffer_p2: $(common_o)
	$(Pecho) "  CXX   $@"
	$P $(CXX) $(CXXFLAGS) \
		-D _USE_USART1_ \
		test_echo_rxcbuffer.cpp \
		-L$(LIB_DIR) -l$(LIBNAME) $(LFLAGS) \
		-o $(BUILD_DIR)/$@.elf

## CleanUp and tools
#
clean:
	$(Pecho) "  CLEAN"
	$P $(RM) $(BUILD_DIR)/test*
