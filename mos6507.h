/*
 * File: mos6507.h
 * Author: dgrubb
 * Date: 06/23/2017
 *
 * Provides implementations of the 6507 model.
 */

#ifndef _MOS6507_H
#define _MOS6507_H

#include <stdint.h>
#include "mos6507-opcodes.h"

#define STATUS_FLAG_NEGATIVE  0x80
#define STATUS_FLAG_OVERFLOW  0x40
#define STATUS_FLAG_BREAK     0x10
#define STATUS_FLAG_DECIMAL   0x08
#define STATUS_FLAG_INTERRUPT 0x04
#define STATUS_FLAG_ZERO      0x02
#define STATUS_FLAG_CARRY     0x01

typedef enum {
    MOS6507_REG_A = 0,
    MOS6507_REG_Y,
    MOS6507_REG_X,
    MOS6507_REG_PC,
    MOS6507_REG_S,
    MOS6507_REG_P
} mos6507_register_t;

typedef struct {
    /* Accumulator */
    uint8_t A;
    /* Y index */
    uint8_t Y;
    /* X index */
    uint8_t X;
    /* Program counter */
    uint16_t PC;
    /* Stack pointer */
    uint8_t S;
    /* Status register */
    uint8_t P;
    /* Currently executing op-code and addressing mode */
    instruction_t current_instruction;
} mos6507;

void mos6507_reset();
void mos6507_set_register(mos6507_register_t reg, uint8_t value);
void mos6507_get_register(mos6507_register_t reg, uint8_t *value);
void mos6507_increment_PC();

#endif /* _MOS6507_H */
