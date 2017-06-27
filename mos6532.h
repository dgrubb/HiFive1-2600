/*
 * File: mos6532.h
 * Author: dgrubb
 * Date: 06/26/2017
 *
 * Provides implementation of MOS 6532 RAM-I/O-Timer (RIOT) chip. This handles
 * both 128 bytes of RAM storage and digital I/O.
 */

#ifndef _MOS6532_H
#define _MOS6532_H

#define MEM_SIZE 128

#include <stdint.h>

uint8_t memory[MEM_SIZE];

/* Utility functions */
int mos6532_bounds_check(uint8_t address);
void mos6532_clear_memory();
/* External memory access */
int mos6532_read(uint8_t *data);
int mos6532_write();

#endif /* _MOS6532_H */

