/*
 * File: debug.c
 * Author: dgrubb
 * Date: 08/07/2017
 *
 * Provides helper functions for printing human-readable
 * information.
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../atari/Atari-memmap.h"
#include "../cpu/mos6507.h"
#include "../memory/mos6532.h"
#define MOS6507_STATUS_FLAG_NEGATIVE  0x80
#define MOS6507_STATUS_FLAG_OVERFLOW  0x40
#define MOS6507_STATUS_FLAG_BREAK     0x10
#define MOS6507_STATUS_FLAG_DECIMAL   0x08
#define MOS6507_STATUS_FLAG_INTERRUPT 0x04
#define MOS6507_STATUS_FLAG_ZERO      0x02
#define MOS6507_STATUS_FLAG_CARRY     0x01

#define MSG_LEN 300

int debug_get_status_flag(uint8_t flag)
{
    uint8_t p;
    mos6507_get_register(MOS6507_REG_P, &p);
    switch (flag) {
        case MOS6507_STATUS_FLAG_NEGATIVE:
            return (p & MOS6507_STATUS_FLAG_NEGATIVE);
        case MOS6507_STATUS_FLAG_OVERFLOW:
            return (p & MOS6507_STATUS_FLAG_OVERFLOW);
        case MOS6507_STATUS_FLAG_BREAK:
            return (p & MOS6507_STATUS_FLAG_BREAK);
        case MOS6507_STATUS_FLAG_DECIMAL:
            return (p & MOS6507_STATUS_FLAG_DECIMAL);
        case MOS6507_STATUS_FLAG_INTERRUPT:
            return (p & MOS6507_STATUS_FLAG_INTERRUPT);
        case MOS6507_STATUS_FLAG_ZERO:
            return (p & MOS6507_STATUS_FLAG_ZERO);
        case MOS6507_STATUS_FLAG_CARRY:
            return (p & MOS6507_STATUS_FLAG_CARRY);
        default:
            return 0;
    }
}

void debug_print_status_flags()
{
    char msg[MSG_LEN];
    memset(msg, 0, MSG_LEN);

    char *template = "Processor flags:\n\r"
                     "|- Sign -|- Overflow -|- Break -|- Decimal -|- Interrupt -|- Zero -|- Carry -|\n\r"
                     "|    %d   |     %d      |    %d    |     %d     |      %d      |   %d    |    %d    |\n\r";

    sprintf(msg, template,
            debug_get_status_flag(MOS6507_STATUS_FLAG_NEGATIVE),
            debug_get_status_flag(MOS6507_STATUS_FLAG_OVERFLOW),
            debug_get_status_flag(MOS6507_STATUS_FLAG_BREAK),
            debug_get_status_flag(MOS6507_STATUS_FLAG_DECIMAL),
            debug_get_status_flag(MOS6507_STATUS_FLAG_INTERRUPT),
            debug_get_status_flag(MOS6507_STATUS_FLAG_ZERO),
            debug_get_status_flag(MOS6507_STATUS_FLAG_CARRY),
            debug_get_status_flag(MOS6507_STATUS_FLAG_CARRY));

    puts(msg);
}

