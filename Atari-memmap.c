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

/* These read/write functions essentially replicate the 
 * chip-select pins on the TIA and RIOT by delegating 
 * memory access requests to their respective locations.
 */
void memmap_write()
{
    // Fetch data and address from CPU
    uint16_t address;
    uint8_t data;
    mos6507_get_data_bus(&data);
    mos6507_get_address_bus(&address);

    // Access particular device
    if (address >= MEMMAP_TIA_START && address <= MEMMAP_TIA_END) {
    } else if (address >= MEMMAP_RIOT_START && address <= MEMMAP_RIOT_END) {
        mos6532_write(address - MEMMAP_RIOT_START, data);
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
        mos6532_read(address - MEMMAP_RIOT_START, data);
    } else if (address >= MEMMAP_CART_START && address <= MEMMAP_CART_END) {
    } else {
        /* Handle error */
    }
}
