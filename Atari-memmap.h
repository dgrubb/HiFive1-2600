/*
 * File: Atari-memmap.h
 * Author: dgrubb
 * Date: 06/28/2017
 *
 * Delegates access to memory mapped devices.
 */

#ifndef _MEMMAP_H
#define _MEMMAP_H

#include <stdint.h>

#define MEM_TIA_START  0x0000
#define MEM_TIA_END    0x007F
#define MEM_RIOT_START 0x0080
#define MEM_RIOT_END   0x0297
#define MEM_CART_START 0xF000
#define MEM_CART_END   0xFFFF


void memmap_write(uint16_t address, uint8_t value);
void memmap_read(uint16_t address, uint8_t *value);

#endif /* _MEMMAP_H */
