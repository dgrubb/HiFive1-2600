/*
 * File: Atari-TIA.c
 * Author: dgrubb
 * Date: 09/18/2017
 *
 * Implements the TIA memory map.
 */

#include "Atari-TIA.h"

int line_count;
tia_pixel tia_line_buffer[TIA_COLOUR_CLOCK_VISIBLE];
atari_tia tia;

/* Resets the TIA instance to default conditions with no state set.
 */
void TIA_init(void)
{
    line_count = 0;
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
 * reg: register to write to (e.g., CXM0P, CXM1P etc ...) value: value to place into register.
 */
void TIA_write_register(uint8_t reg, uint8_t value)
{
    tia.write_regs[reg] = value;
}

void TIA_generate_colour(void)
{

}

void TIA_write_to_buffer(tia_pixel pixel, int pixel_index)
{
    if (TIA_COLOUR_CLOCK_TOTAL > pixel_index) {
        tia_line_buffer[pixel_index] = pixel;
    }
}

void TIA_clock_tick(void)
{
    /* Vertical blanking periods */
    /* TODO: reset line_count after overscan */

    /* Reset colour clock and prepare begin next line */
    if (tia.colour_clock == TIA_COLOUR_CLOCK_TOTAL) {
        tia.colour_clock = 0;
        tia.write_regs[TIA_WRITE_REG_WSYNC] = 0;
        line_count++;
        return;
    }
    if (tia.colour_clock > TIA_COLOUR_CLOCK_HSYNC) {
        TIA_generate_colour();
    } else {
        /* Horizontal sync time */
    }
    tia.colour_clock++;
}

int TIA_get_WSYNC()
{
    return (tia.write_regs[TIA_WRITE_REG_WSYNC] ? 1 : 0);
}
