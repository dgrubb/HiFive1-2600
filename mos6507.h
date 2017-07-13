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

#define MOS6507_STATUS_FLAG_NEGATIVE  0x80
#define MOS6507_STATUS_FLAG_OVERFLOW  0x40
#define MOS6507_STATUS_FLAG_BREAK     0x10
#define MOS6507_STATUS_FLAG_DECIMAL   0x08
#define MOS6507_STATUS_FLAG_INTERRUPT 0x04
#define MOS6507_STATUS_FLAG_ZERO      0x02
#define MOS6507_STATUS_FLAG_CARRY     0x01

typedef enum {
    MOS6507_REG_A = 0,
    MOS6507_REG_Y,
    MOS6507_REG_X,
    MOS6507_REG_PC,
    MOS6507_REG_S,
    MOS6507_REG_P
} mos6507_register_t;

typedef struct {
    /* Internal registers */
    uint8_t  A;   /* Accumulator */
    uint8_t  Y;   /* Y register */
    uint8_t  X;   /* X register */
    uint16_t PC; /* Program counter */
    uint8_t  S;   /* Stack pointer */
    uint8_t  P;   /* Status register */
    /* State description */
    uint8_t       current_instruction; /* Current op-code and addressing mode */
    uint16_t      address_bus;         /* Address bus */
    uint8_t       data_bus;            /* Data bus */
} mos6507;

void mos6507_init();
void mos6507_reset();
void mos6507_clock();
void mos6507_set_register(mos6507_register_t reg, uint8_t value);
void mos6507_get_register(mos6507_register_t reg, uint8_t *value);
void mos6507_set_address_bus_hl(uint8_t adh, uint8_t adl);
void mos6507_set_address_bus(uint16_t address);
void mos6507_get_address_bus(uint16_t *address);
void mos6507_set_data_bus(uint8_t data);
void mos6507_get_data_bus(uint8_t *data);
void mos6507_increment_PC();
uint16_t mos6507_get_PC();
void mos6507_set_PC(uint16_t pc);

#endif /* _MOS6507_H */
