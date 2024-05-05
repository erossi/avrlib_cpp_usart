# USART - Serial port library.
# Copyright (C) 2017, 2020, 2024 Enrico Rossi
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

LIB_NAME = usart

# Overridable parameters
export OPTLEV ?= 2
export MCU ?= atmega1284p
export FCPU ?= 1000000UL
export DMACRO ?=

PWD = $(shell pwd)
INCLUDE_DIR := $(PWD)/../include
LIB_DIR := $(PWD)/../lib
INC = -I../modules/circular_buffer/include/
INC += -I$(INCLUDE_DIR)/

# Compilers
AR := avr-ar
CXX := avr-g++
BUILD_DIR := /run/user/$(shell id --user)/build
CXXFLAGS := $(INC) \
	    -Wall \
	    -Wextra \
	    -pedantic \
	    -std=c++14 \
	    -mmcu=$(MCU) \
	    -O$(OPTLEV) \
	    -fshort-enums \
	    $(DMACRO) -D F_CPU=$(FCPU)
LFLAGS = -lm

# Uncomment if git is in use
GIT_TAG = $(shell git describe --tags --long --always)
LIBNAME := $(LIB_NAME)-$(GIT_TAG)-$(MCU)
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

# Objects
common_o := \
	    usart_base.o \
	    usart_base_p2.o \
	    usart_rxcbuffer.o \
	    usart_rxcbuffer_p2.o

# Default target.
all: lib

# Creates the object directory
$(BUILD_DIR):
	$P mkdir -p $(BUILD_DIR)

$(LIB_DIR):
	$P mkdir -p $(LIB_DIR)

$(INCLUDE_DIR):
	$P mkdir -p $(INCLUDE_DIR)

lib: $(BUILD_DIR) $(LIB_DIR) $(INCLUDE_DIR) $(common_o)
	$(Pecho) "  AR    lib$(LIB_NAME).a"
	$P $(AR) rcs $(LIB_DIR)/lib$(LIBNAME).a $(addprefix $(BUILD_DIR)/, $(common_o))
	$P cd $(LIB_DIR) && ln -sf lib$(LIBNAME).a lib$(LIB_NAME).a
	$(Pecho) "  INST  headers"
	$P cd $(INCLUDE_DIR) && cp ../src/*.h .
	$P cd $(INCLUDE_DIR) && cp ../modules/circular_buffer/include/*.h .

tests: lib
	cd $(PWD)/../tests && $(MAKE)

# Standard build
%.o: %.c
	$(Pecho) "  CC    $<"
	$P $(CC) -MMD -c $(CFLAGS) $< -o $(BUILD_DIR)/$@

%.o: %.cpp
	$(Pecho) "  CXX   $<"
	$P $(CXX) -MMD -c $(CXXFLAGS) $< -o $(BUILD_DIR)/$@

## CleanUp and tools
#
clean:
	$P cd $(PWD)/../tests && $(MAKE) $@
	$(Pecho) "  CLEAN"
	$P $(RM) -r $(LIB_DIR)
	$P $(RM) -r $(INCLUDE_DIR)
	$P $(RM) -r $(BUILD_DIR)
