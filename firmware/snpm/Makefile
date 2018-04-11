##
## This file is part of the libopencm3 project.
##
## Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
## Copyright (C) 2010 Piotr Esden-Tempski <piotr@esden.net>
##
## This library is free software: you can redistribute it and/or modify
## it under the terms of the GNU Lesser General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public License
## along with this library.  If not, see <http://www.gnu.org/licenses/>.
##


BINARY = main

LDSCRIPT = ../stm32f0.ld

# To enable semihosting debug, run "make ENABLE_SEMIHOSTING=1" or comment out next line
# ENABLE_SEMIHOSTING ?= 1

ifeq ($(ENABLE_SEMIHOSTING),1)
LDFLAGS         += --specs=rdimon.specs
LDLIBS          += -lrdimon
DEFS            += -DENABLE_SEMIHOSTING=1
endif

LIBNAME		= opencm3_stm32f0
DEFS		+= -DSTM32F0

FP_FLAGS	?= -msoft-float
ARCH_FLAGS	= -mthumb -mcpu=cortex-m0 $(FP_FLAGS)

################################################################################
# OpenOCD specific variables

OOCD		?= openocd
OOCD_INTERFACE	?= stlink-v2-1
OOCD_TARGET	?= stm32f0x

################################################################################
# Black Magic Probe specific variables
# Set the BMP_PORT to a serial port and then BMP is used for flashing
BMP_PORT	?=

################################################################################
# texane/stlink specific variables
#STLINK_PORT	?= :4242


include ../rules.mk

