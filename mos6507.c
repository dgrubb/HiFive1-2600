/*
 * File: mos6507.c
 * Author: dgrubb
 * Date: 06/23/2017
 *
 * Provides implementations of the 6507 model.
 */

#include "mos6507.h"

/* Representation of our CPU */
static mos6507 cpu;

void mos6507_reset()
{
    /* Initialise all members back to 0 */
    mos6507 cpu = {0};
}

void mos6507_set_register(mos6507_register_t reg, uint8_t value)
{
    switch(reg) {
        case MOS6507_REG_A: cpu.A = value; break;
        case MOS6507_REG_Y: cpu.Y = value; break;
        case MOS6507_REG_X: cpu.X = value; break;
        case MOS6507_REG_PC: cpu.PC = value; break;
        case MOS6507_REG_S: cpu.S = value; break;
        case MOS6507_REG_P: cpu.P = value; break;
        default: /* Handle error */ break;
    }
}

void mos6507_get_register(mos6507_register_t reg, uint8_t *value)
{
    switch(reg) {
        case MOS6507_REG_A: *value = cpu.A; break;
        case MOS6507_REG_Y: *value = cpu.Y; break;
        case MOS6507_REG_X: *value = cpu.X; break;
        case MOS6507_REG_PC: *value = cpu.PC; break;
        case MOS6507_REG_S: *value = cpu.S; break;
        case MOS6507_REG_P: *value = cpu.P; break;
        default: /* Handle error */ break;
    }
}

void mos6507_increment_PC()
{
    cpu.PC++;
}
