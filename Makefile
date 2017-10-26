TARGET = HiFive1-2600
CFLAGS += -O2 -fno-builtin-printf -DUSE_PLIC -DUSE_M_TIME

# Enable compilation and execution of test suite
CFLAGS += -DEXEC_TESTS
# Enable manual stepping through a cartridge, requires EXEC_TESTS for
# debug helper functions
CFLAGS += -DMANUAL_STEP
# Run program without the requirement of a slave TIA device
#CFLAGS += -DSTANDALONE

CFLAGS += -I./

BSP_BASE = ../../bsp

# CPU:
C_SRCS += cpu/mos6507.c
C_SRCS += cpu/mos6507-opcodes.c
C_SRCS += cpu/mos6507-microcode.c
# Memory:
C_SRCS += memory/mos6532.c
# System architecture:
C_SRCS += atari/Atari-memmap.c
C_SRCS += atari/Atari-cart.c
C_SRCS += atari/Atari-TIA.c
# uC hardware
C_SRCS += external/spi.c
C_SRCS += external/UART_driver.c
# Program logic
C_SRCS += test/debug.c
C_SRCS += test/test-carts.c
C_SRCS += carts/halo2600.c
C_SRCS += test/tests.c
C_SRCS += main.c

C_SRCS += $(BSP_BASE)/drivers/plic/plic_driver.c

include $(BSP_BASE)/env/common.mk
