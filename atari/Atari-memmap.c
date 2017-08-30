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

#define IS_TIA(x) \
    (x >= MEMMAP_TIA_START && x <= MEMMAP_TIA_END) ? 1 : 0


#define IS_RIOT(x) \
    (x >= MEMMAP_RIOT_START && x <= MEMMAP_TIA_END) ? 1 : 0

#define IS_CART(x) \
    (x >= MEMMAP_CART_START && x <= MEMMAP_CART_END) ? 1 : 0

void memmap_map_address(uint16_t *address)
{
    /* The 6507 is a variant of the 6502. It shares the 16-bit addressing
     * system internally, but only connects to 13 external pins so the 
     * largest address which can be selected is 0x1FFF. However, as the 
     * processor can still handle full 16-bit addresses a programmer may have
     * used a greater value with the expectation those bits would be disregarded
     * and mirroring would take place.
     */
    *address = (*address & 0x1FFF);
}

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
    memmap_map_address(&address);

    /* Access particular device */
    if (IS_TIA(address)) {
        return;
    }
    if (IS_RIOT(address)) {
        mos6532_write(address - MEMMAP_RIOT_START, data);
        return;
    }
    if (IS_CART(address)) {
        /* Cartridges are read-only. Are there hardware peripherals which 
         * use this space for extending functionality? E.g., SuperCharger?
         */
        return;
    }
}

void memmap_read(uint8_t *data)
{
    /* Fetch address from CPU */
    uint16_t address;
    mos6507_get_address_bus(&address);
    memmap_map_address(&address);

    /* Access particular device */
    if (IS_TIA(address)) {
        return;
    }
    if (IS_RIOT(address)) {
        mos6532_read(address - MEMMAP_RIOT_START, data);
        return;
    }
    if (IS_CART(address)) {
        cartridge_read(address - MEMMAP_CART_START, data);
        return;
    }
}

