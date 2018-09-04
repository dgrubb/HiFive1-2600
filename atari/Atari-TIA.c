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
 * for easier alignment. See page 43 of docs/Stella Programmer's guide.pdf
 */
tia_pixel_t tia_colour_map[] = {
                          /* TIA:  >>1:  Index (dec): */
    { 0x00, 0x00, 0x00 }, /* 0x00, 0x00, 0 */
    { 0x1A, 0x1A, 0x1A }, /* 0x02, 0x01, 1 */
    { 0x39, 0x39, 0x39 }, /* 0x04, 0x02, 2 */
    { 0x58, 0x58, 0x58 }, /* 0x06, 0x03, 3 */
    { 0x7E, 0x7E, 0x7E }, /* 0x08, 0x04, 4 */
    { 0xA2, 0xA2, 0xA2 }, /* 0x0A, 0x05, 5 */
    { 0xC7, 0xC7, 0xC7 }, /* 0x0C, 0x06, 6 */
    { 0xED, 0xED, 0xED }, /* 0x0E, 0x07, 7 */
    { 0x19, 0x92, 0x00 }, /* 0x10, 0x08, 8 */
    { 0x3A, 0x1F, 0x00 }, /* 0x12, 0x09, 9 */
    { 0x5D, 0x41, 0x00 }, /* 0x14, 0x0A, 10 */
    { 0x82, 0x64, 0x00 }, /* 0x16, 0x0B, 11 */
    { 0xA7, 0x88, 0x00 }, /* 0x18, 0x0C, 12 */
    { 0xCC, 0xAD, 0x00 }, /* 0x1A, 0x0D, 13 */
    { 0xF2, 0xD2, 0x19 }, /* 0x1C, 0x0E, 14 */
    { 0xFE, 0xFA, 0x40 }, /* 0x1E, 0x0F, 15 */
    { 0x37, 0x00, 0x00 }, /* 0x20, 0x10, 16 */
    { 0x5E, 0x08, 0x00 }, /* 0x22, 0x11, 17 */
    { 0x83, 0x27, 0x00 }, /* 0x24, 0x12, 18 */
    { 0xA9, 0x49, 0x00 }, /* 0x26, 0x13, 19 */
    { 0xCF, 0x6C, 0x00 }, /* 0x28, 0x14, 20 */
    { 0xF5, 0x8F, 0x17 }, /* 0x2A, 0x15, 21 */
    { 0xFE, 0xB4, 0x38 }, /* 0x2C, 0x16, 22 */
    { 0xFE, 0xDF, 0x6F }, /* 0x2E, 0x17, 23 */
    { 0x47, 0x00, 0x00 }, /* 0x30, 0x18, 24 */
    { 0x73, 0x00, 0x00 }, /* 0x32, 0x19, 25 */
    { 0x98, 0x13, 0x00 }, /* 0x34, 0x1A, 26 */
    { 0xBE, 0x32, 0x16 } /* 0x36, 0x1B, 27 */
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
