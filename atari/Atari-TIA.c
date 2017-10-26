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
void TIA_init(void)
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
void TIA_read_register(uint8_t reg, uint8_t *value)
{
    *value = tia.read_regs[reg];
}

/* Writes a value into a register location
 *
 * reg: register to write to (e.g., CXM0P, CXM1P etc ...).
 * value: value to place into register.
 */
void TIA_write_register(uint8_t reg, uint8_t value)
{
    tia.write_regs[reg] = value;
}

void TIA_generate_colour(void)
{

}

void TIA_generate_hsync(void)
{
}

void TIA_generate_vsync(void)
{
}

void TIA_generate_vblank(void)
{
}

void TIA_clock_tick(void)
{
    /* Vertical blanking periods */
    if (tia.write_regs[TIA_WRITE_REG_VSYNC]) {
        TIA_generate_vsync();
        return;
    }
    if (tia.write_regs[TIA_WRITE_REG_VBLANK]) {
        TIA_generate_vblank();
        return;
    }

    /* Horizonal colour and blanking signals  */
    if (tia.colour_clock == TIA_COLOUR_CLOCK_TOTAL) {
        tia.colour_clock = 0;
        return;
    }
    if (tia.colour_clock <= TIA_COLOUR_CLOCK_VISIBLE) {
        TIA_generate_colour();
    }
    if ((tia.colour_clock > TIA_COLOUR_CLOCK_VISIBLE) && (tia.colour_clock < TIA_COLOUR_CLOCK_TOTAL)) {
        TIA_generate_hsync();
    }
    tia.colour_clock++;
}

