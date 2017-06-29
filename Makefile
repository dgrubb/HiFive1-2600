TARGET = HiFive1-2600
CFLAGS += -O2 -fno-builtin-printf -DUSE_PLIC -DUSE_M_TIME

BSP_BASE = ../../bsp

# Program logic
C_SRCS += main.c
# IC emulation
C_SRCS += mos6507.c
C_SRCS += mos6507-opcodes.c
C_SRCS += mos6507-microcode.c
C_SRCS += mos6532.c
C_SRCS += Atari-TIA.c
C_SRCS += Atari-memmap.c
C_SRCS += Atari-cart.c

C_SRCS += $(BSP_BASE)/drivers/plic/plic_driver.c

include $(BSP_BASE)/env/common.mk
