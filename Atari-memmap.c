/*
 * File: Atari-memmap.c
 * Author: dgrubb
 * Date: 06/28/2017
 *
 * Delegates access to memory mapped devices.
 */

#include "Atari-memmap.h"
#include "mos6507.h"
#include "mos6532.h"

void memmap_write()
{
    uint16_t address;
    if (address >= MEMMAP_TIA_START && address <= MEMMAP_TIA_END) {
    } else if (address >= MEMMAP_RIOT_START && address <= MEMMAP_RIOT_END) {
    } else if (address >= MEMMAP_CART_START && address <= MEMMAP_CART_END) {
    } else {
        /* Handle error */
    }
}

void memmap_read(uint8_t *data)
{
    uint16_t address;
    if (address >= MEMMAP_TIA_START && address <= MEMMAP_TIA_END) {
    } else if (address >= MEMMAP_RIOT_START && address <= MEMMAP_RIOT_END) {
    } else if (address >= MEMMAP_CART_START && address <= MEMMAP_CART_END) {
    } else {
        /* Handle error */
    }
}
