TARGET = HiFive1-2600
CFLAGS += -O2 -fno-builtin-printf -DUSE_PLIC -DUSE_M_TIME

# Enable compilation and execution of test suite
CFLAGS += -DEXEC_TESTS
# Run program without the requirement of a slave TIA device
CFLAGS += -DSTANDALONE

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
# uC hardware
C_SRCS += external/spi.c
# Program logic
C_SRCS += test/debug.c
C_SRCS += test/tests.c
C_SRCS += main.c

C_SRCS += $(BSP_BASE)/drivers/plic/plic_driver.c

include $(BSP_BASE)/env/common.mk
