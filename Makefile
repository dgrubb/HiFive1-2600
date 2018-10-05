# File: Makefile
# Author: dgrubb
# Date: 10/26/2017
#
# Usage:
#
# Incorporates into SiFive's freedom-e-sdk build process so no 
# manual invocation is required. See freedom-e-sdk/Makefile for 
# more information.

# Output binary
TARGET = HiFive1-2600

###############################################################################
# Compilation flags
###############################################################################

# Set optimisation level
CFLAGS += -Os

# Warn that printf is custom implementation
CFLAGS += -fno-builtin-printf

CFLAGS += -mexplicit-relocs
CFLAGS += -falign-functions=4
CFLAGS += -DNOENUM

# Use PLIC to control external interrupts
CFLAGS += -DUSE_PLIC

# Use timer interrupt callback
CFLAGS += -DUSE_M_TIME

# Enable compilation and execution of test suite
# CFLAGS += -DEXEC_TESTS

# Enable manual stepping through a cartridge
# CFLAGS += -DMANUAL_STEP

# Enable displaying the TIA colour map on screen
# CFLAGS += -DCOLOUR_TEST

# Allow for printing the emulator state to UART
# CFLAGS += -DPRINT_STATE

# Identify this directory for location of custom headers
CFLAGS += -I./

###############################################################################
# Sources
###############################################################################

BSP_BASE = ../../bsp
# CPU emulation
C_SRCS += mos6507/mos6507.c
C_SRCS += mos6507/mos6507-opcodes.c
C_SRCS += mos6507/mos6507-microcode.c
# Memory and I/O chip (RIOT) emulation
C_SRCS += mos6532/mos6532.c
# System architecture
C_SRCS += atari/Atari-memmap.c
C_SRCS += atari/Atari-cart.c
C_SRCS += atari/Atari-TIA.c
# uC hardware
C_SRCS += external/spi.c
C_SRCS += external/UART_driver.c
C_SRCS += external/ili9341.c
C_SRCS += external/platform_util.c
# Program logic
C_SRCS += test/debug.c
C_SRCS += test/test-carts.c
C_SRCS += carts/halo2600.c
C_SRCS += test/tests.c
C_SRCS += main.c

C_SRCS += $(BSP_BASE)/drivers/plic/plic_driver.c

include $(BSP_BASE)/env/common.mk
