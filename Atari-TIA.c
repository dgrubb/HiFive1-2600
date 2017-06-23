/*
 * File: Atari-TIA.c
 * Author: dgrubb
 * Date: 06/23/2017
 *
 * Implements the Atari 2600's custom Television Interface Adapter chip. This
 * provides audio and graphics drawing via analog beam chasing methods (as
 * opposed to a stored framebuffer). The TIA also provides features for
 * providing simple game objects such as playfield, players and missiles.
 *
 * References:
 *
 * Stella Programmer's Guide - http://atarihq.com/danb/files/stella.pdf
 * Chip pinout - http://atarihq.com/danb/tia.shtml
 */

#include "Atari-TIA.h"

/* Singleton representation of a TIA chip */
atari_tia tia;

/* Resets the TIA instance to default conditions with no state set.
 */
void TIA_init()
{
    int i = 0;
    // I/O interfaces
    tia.address_bus = 0;
    tia.data_bus = 0;
    tia.databus_direction = WRITE;
    tia.chip_select = 0;
    // Data registers
    for (i=0; i<WRITABLE_REG_LEN; i++) {
        tia.write_regs[i] = 0;
    }
    for (i=0; i<READABLE_REG_LEN; i++) {
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
