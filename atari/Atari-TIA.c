/*
 * File: Atari-TIA.c
 * Author: dgrubb
 * Date: 09/18/2017
 *
 * Implements the TIA memory map.
 */

#include "Atari-TIA.h"

/* Singleton representation of a TIA chip */
atari_tia tia;

/* Resets the TIA instance to default conditions with no state set.
 */
void TIA_init()
{
    int i = 0;
    // Data registers
    for (i=0; i<TIA_WRITE_REG_LEN; i++) {
        tia.write_regs[i] = 0;
    }
    for (i=0; i<TIA_READ_REG_LEN; i++) {
        tia.read_regs[i] = 0;
    }
}

/* Retrieves a value in a specified register
 *
 * reg: register to read (e.g., VSYNC, VBLANK, etc ...).
 * *value: location to place retrieved value of register into.
 */
void TIA_read_register(tia_readable_register_t reg, uint8_t *value)
{
    *value = tia.read_regs[reg];
}

/* Writes a value into a register location
 *
 * reg: register to write to (e.g., CXM0P, CXM1P etc ...).
 * value: value to place into register.
 */
void TIA_write_register(tia_writable_register_t reg, uint8_t value)
{
    tia.write_regs[reg] = value;
}

