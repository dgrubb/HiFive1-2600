/*
 * File: mos6507-microcode.c
 * Author: dgrubb
 * Date: 06/27/2017
 *
 * Provides implementation of CPU logic without reference to
 * specific addressing modes.
 */

#include "mos6507-microcode.h"
#include "mos6507.h"



void mos6507_ADC(uint8_t data)
{
    uint8_t accumulator, status = 0;
    mos6507_get_register(MOS6507_REG_A, &accumulator);
    mos6507_get_register(MOS6507_REG_P, &status);

    
}

void mos6507_AND(uint8_t data)
{
}

void mos6507_ASL(uint8_t data)
{
}

void mos6507_BIT(uint8_t data)
{
}

void mos6507_CMP(uint8_t data)
{
}

void mos6507_CPX(uint8_t data)
{
}

void mos6507_CPY(uint8_t data)
{
}

void mos6507_EOR(uint8_t data)
{
}

void mos6507_LSR(uint8_t data)
{
}

void mos6507_ORA(uint8_t data)
{
}

void mos6507_ROL(uint8_t data)
{
}

void mos6507_ROR(uint8_t data)
{
}

void mos6507_SBC(uint8_t data)
{
}
