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
static uint8_t *cartridge[];

void read_cartridge(uint16_t address, uint8_t * data)
{
}

void load_cartridge(uint8_t *cart[])
{
}

void eject_cartridge();
