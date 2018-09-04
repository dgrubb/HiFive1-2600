/*
 * File: Atari-TIA.c
 * Author: dgrubb
 * Date: 09/18/2017
 *
 * Implements the TIA memory map.
 */

#include "Atari-TIA.h"

int line_count;
//tia_pixel_t tia_line_buffer[TIA_COLOUR_CLOCK_VISIBLE];
atari_tia tia;

/* Usage note:
 *
 * This table is mapped to allow for lookup from a value in one of the colour
 * registers. However, the colour reigsters don't use the least significant
 * bit so the table locations assume the value has been shifted by one bit
 * for easier alignment.
 */
tia_pixel_t tia_colour_map[] = {
    { 0x00, 0x00, 0x00 }, /* 0x00 */
    { 0x1A, 0x1A, 0x1A }, /* 0x01 */
    { 0x39, 0x39, 0x39 }, /* 0x */
    {}
};

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
    /* TODO check order of priority established in PFB bits
     * to establish if playfield need to be rendered over player
     * objects
     */

    


    /* TODO check for collisions and set registers appropriately */
}

void TIA_write_to_buffer(tia_pixel_t pixel, int pixel_index)
{
    if (TIA_COLOUR_CLOCK_TOTAL > pixel_index) {
        tia_line_buffer[pixel_index] = pixel;
    }
}

void TIA_colour_to_RGB(tia_pixel_t* pixel, uint32_t *rgb)
{

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
