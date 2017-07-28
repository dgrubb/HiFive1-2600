/*
 * File: Atari-cart.h
 * Author: dgrubb
 * Date: 07/07/2017
 *
 * Mimics ROM space (that is, a game cartridge).
 */

#ifndef _ATARI_CART_H
#define _ATARI_CART_H

#include <stdint.h>

void read_cartridge(uint16_t address, uint8_t * data);
void load_cartridge(uint8_t cartridge);
void eject_cartridge();

#endif /* _ATARI_CART_H */
