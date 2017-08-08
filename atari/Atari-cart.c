/*
 * File: Atari-cart.c
 * Author: dgrubb
 * Date: 07/07/2017
 *
 * Mimics ROM space (that is, a game cartridge).
 */

#include "Atari-cart.h"

/* Cartridges are represented as arrays of bytes in their own
 * part of memory. We "load" a cartridge by storing a pointer 
 * to the desired cartridge data.
 */
static uint8_t *cartridge;

void cartridge_read(uint16_t address, uint8_t * data)
{
    // TODO: this is really dangerous, fix please
    *data = cartridge[address];
}

void cartridge_load(uint8_t *cart)
{
    cartridge = cart;
}

void cartridge_eject();