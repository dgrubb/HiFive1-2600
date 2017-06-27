/*
 * File: mos6532.c
 * Author: dgrubb
 * Date: 06/26/2017
 *
 * Provides implementation of MOS 6532 RAM-I/O-Timer (RIOT) chip. This handles
 * both 128 bytes of RAM storage and digital I/O.
 */

#include <string.h>
#include "mos6532.h"
#include "mos6507.h"

/* Checks that a requested memory address is within the 
 * RAM space.
 *
 * address: address to verify.
 *
 * Returns 0 on success, -1 on error.
 */
int mos6532_bounds_check(uint8_t address)
{
    if (address > MEM_SIZE || address < 0) {
        return -1;
    }
    /* Flag an error */
    return 0;
}

/* Resets all RAM to zero
 */
void mos6532_clear_memory()
{
    memset(memory, 0, MEM_SIZE);
}

/* Loads a value from within RAM and places it into 
 * a variable given by pointer.
 *
 * Returns 0 on success, -1 on error.
 */
int mos6532_read(uint8_t *data)
{
    uint16_t address = 0;
    mos6507_get_address_bus(&address);
    if (-1 == mos6532_bounds_check(address)) {
        /* Error, attempting to write outside memory */
        return -1;
    }
    *data = memory[address];
    return 0;
}

/* Writes to a RAM address.
 *
 * Returns 0 on success, -1 on error.
 */
int mos6532_write()
{
    uint16_t address = 0;
    uint8_t data = 0;
    mos6507_get_address_bus(&address);
    mos6507_get_data_bus(&data);
    if (-1 == mos6532_bounds_check(address)) {
        /* Error, attempting to write outside memory */
        return -1;
    }
    memory[address] = data;
    return 0;
}
