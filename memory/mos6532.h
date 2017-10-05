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

#include <stdint.h>

#define MEM_SIZE 128
#define MOS6532_TIM1T_INTERVAL  1
#define MOS6532_TIM8T_INTERVAL  8
#define MOS6532_TIM64T_INTERVAL 64
#define MOS6532_T1024T_INTERVAL 1024

uint8_t memory[MEM_SIZE];

/* Utility functions */
int mos6532_bounds_check(uint8_t address);
void mos6532_clear_memory();
/* External memory access */
int mos6532_read(uint16_t address, uint8_t *data);
int mos6532_write(uint16_t address, uint8_t data);
void mos6532_clock_tick();

#endif /* _MOS6532_H */

