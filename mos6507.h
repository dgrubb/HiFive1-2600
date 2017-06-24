/*
 * File: mos6507.h
 * Author: dgrubb
 * Date: 06/23/2017
 *
 * Provides implementations of the 6507 model.
 */

#ifndef _MOS6507_H
#define _MOS6507_H

#include <stdint.h>

#define STATUS_FLAG_NEGATIVE  0x80
#define STATUS_FLAG_OVERFLOW  0x40
#define STATUS_FLAG_BREAK     0x10
#define STATUS_FLAG_DECIMAL   0x08
#define STATUS_FLAG_INTERRUPT 0x04
#define STATUS_FLAG_ZERO      0x02
#define STATUS_FLAG_CARRY     0x01

typedef struct {
    /* Accumulator */
    uint8_t A;
    /* Y index */
    uint8_t Y;
    /* X index */
    uint8_t X;
    /* Program counter */
    uint16_t PC;
    /* Stack pointer */
    uint8_t S;
    /* Status register */
    uint8_t P;
} mos6507;

extern mos6507 cpu;

#endif /* _MOS6507_H */
