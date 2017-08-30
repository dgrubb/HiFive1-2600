/*
 * File: Atari-memmap.c
 * Author: dgrubb
 * Date: 06/28/2017
 *
 * Delegates access to memory mapped devices.
 */

#include "Atari-memmap.h"
#include "Atari-cart.h"
#include "../cpu/mos6507.h"
#include "../memory/mos6532.h"

/* These read/write functions essentially replicate the 
 * chip-select pins on the TIA and RIOT by delegating 
 * memory access requests to their respective locations.
 */
void memmap_write()
{
    /* Fetch data and address from CPU */
    uint16_t address;
    uint8_t data;
    mos6507_get_data_bus(&data);
    mos6507_get_address_bus(&address);

    /* Access particular device */
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
    /* Fetch address from CPU */
    uint16_t address;
    mos6507_get_address_bus(&address);

    /* The 6507 is a variant of the 6502. It shares the 16-bit addressing
     * system internally, but only connects to 13 external pins so the 
     * largest address which can be selected is 0x1FFF. However, as the 
     * processor can still handle full 16-bit addresses a programmer may have
     * used a greater value with the expectation those bits would be disregarded
     * and mirroring would take place.
     */
    address = (address & 0x1FFF);

    /* Access particular device */
    if (address >= MEMMAP_TIA_START && address <= MEMMAP_TIA_END) {
    } else if (address >= MEMMAP_RIOT_START && address <= MEMMAP_RIOT_END) {
        mos6532_read(address - MEMMAP_RIOT_START, data);
    } else if (address >= MEMMAP_CART_START && address <= MEMMAP_CART_END) {
        cartridge_read(address - MEMMAP_CART_START, data);
    } else {
        /* Handle error */
    }
}
