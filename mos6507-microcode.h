/*
 * File: mos6507-microcode.h
 * Author: dgrubb
 * Date: 06/27/2017
 *
 * Provides implementation of CPU logic without reference to
 * specific addressing modes.
 */

#ifndef _MOS6507_MICROCODE_H
#define _MOS6507_MICROCODE_H

#include <stdint.h>

typedef enum {
    OPCODE_ADDRESSING_DESTINATION_A,
    OPCODE_ADDRESSING_DESTINATION_X,
    OPCODE_ADDRESSING_DESTINATION_Y,
    OPCODE_ADDRESSING_DESTINATION_MEM
} addressing_destination_t;


void mos6507_ADC(uint8_t data);
void mos6507_AND(uint8_t data);
void mos6507_ASL(uint8_t data, addressing_destination_t destination, uint16_t address);
void mos6507_BIT(uint8_t data);
void mos6507_CMP(uint8_t data);
void mos6507_CPX(uint8_t data);
void mos6507_CPY(uint8_t data);

#endif /* _MOS6507_MICROCODE_H */

