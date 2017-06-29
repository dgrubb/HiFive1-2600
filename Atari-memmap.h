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

#define MEMMAP_TIA_START  0x0000
#define MEMMAP_TIA_END    0x007F
#define MEMMAP_RIOT_START 0x0080
#define MEMMAP_RIOT_END   0x0297
#define MEMMAP_CART_START 0xF000
#define MEMMAP_CART_END   0xFFFF

void memmap_write();
void memmap_read(uint8_t *data);

#endif /* _MEMMAP_H */
