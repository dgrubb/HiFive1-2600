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
    uint16_t tmp;
    uint8_t accumulator = 0;
    mos6507_get_register(MOS6507_REG_A, &accumulator);

    if (mos6507_get_status_flag(MOS6507_STATUS_FLAG_DECIMAL)) {
        // TODO
    } else {
        tmp = data + accumulator + (mos6507_get_status_flag(MOS6507_STATUS_FLAG_CARRY) ? 1 : 0);
        mos6507_set_status_flag(MOS6507_STATUS_FLAG_NEGATIVE, (tmp & 0x80));
        mos6507_set_status_flag(MOS6507_STATUS_FLAG_CARRY, (tmp > 0xFF));
        mos6507_set_status_flag(MOS6507_STATUS_FLAG_ZERO, !(tmp & 0xFF));
        mos6507_set_status_flag(
            MOS6507_STATUS_FLAG_OVERFLOW,
            !((accumulator ^ data) & 0x80) && ((accumulator ^ tmp) & 0x80)
        );
    }

    mos6507_set_register(MOS6507_REG_A, (tmp & 0xFF));
}

void mos6507_AND(uint8_t data)
{
    uint8_t accumulator, tmp = 0;
    mos6507_get_register(MOS6507_REG_A, &accumulator);

    tmp = accumulator & data;
    mos6507_set_status_flag(MOS6507_STATUS_FLAG_NEGATIVE, (tmp & 0x80));
    mos6507_set_status_flag(MOS6507_STATUS_FLAG_ZERO, !(tmp & 0xFF));

    mos6507_set_register(MOS6507_REG_A, tmp);
}

void mos6507_ASL(uint8_t *data)
{
    uint16_t tmp = *data;

    tmp <<= 1;
    mos6507_set_status_flag(MOS6507_STATUS_FLAG_CARRY, (tmp & 0x100));
    mos6507_set_status_flag(MOS6507_STATUS_FLAG_NEGATIVE, (tmp & 0x80));
    mos6507_set_status_flag(MOS6507_STATUS_FLAG_ZERO, !(tmp & 0xFF));

    *data = (tmp & 0xFF);
}

void mos6507_BIT(uint8_t data)
{
    uint8_t accumulator, tmp = 0;
    mos6507_get_register(MOS6507_REG_A, &accumulator);

    tmp = accumulator & data;

    mos6507_set_status_flag(MOS6507_STATUS_FLAG_NEGATIVE, (data & MOS6507_STATUS_FLAG_NEGATIVE));
    mos6507_set_status_flag(MOS6507_STATUS_FLAG_OVERFLOW, (data & MOS6507_STATUS_FLAG_OVERFLOW));
    mos6507_set_status_flag(MOS6507_STATUS_FLAG_ZERO, !(tmp & 0xFF));
}

void mos6507_CMP(uint8_t data)
{
    uint16_t tmp = 0;
    uint8_t accumulator = 0;

    mos6507_get_register(MOS6507_REG_A, &accumulator);

    tmp = accumulator - data;
    mos6507_set_status_flag(MOS6507_STATUS_FLAG_CARRY, (tmp < 0x0100));
    mos6507_set_status_flag(MOS6507_STATUS_FLAG_NEGATIVE, (tmp & 0x80));
    mos6507_set_status_flag(MOS6507_STATUS_FLAG_ZERO, !(tmp & 0xFF));
}

void mos6507_CPX(uint8_t data)
{
    uint16_t tmp = 0;
    uint8_t X = 0;

    mos6507_get_register(MOS6507_REG_X, &X);

    tmp = accumulator - data;
    mos6507_set_status_flag(MOS6507_STATUS_FLAG_CARRY, (tmp < 0x0100));
    mos6507_set_status_flag(MOS6507_STATUS_FLAG_NEGATIVE, (tmp & 0x80));
    mos6507_set_status_flag(MOS6507_STATUS_FLAG_ZERO, !(tmp & 0xFF));
}

void mos6507_CPY(uint8_t data)
{
    uint16_t tmp = 0;
    uint8_t Y = 0;

    mos6507_get_register(MOS6507_REG_Y, &Y);

    tmp = accumulator - data;
    mos6507_set_status_flag(MOS6507_STATUS_FLAG_CARRY, (tmp < 0x0100));
    mos6507_set_status_flag(MOS6507_STATUS_FLAG_NEGATIVE, (tmp & 0x80));
    mos6507_set_status_flag(MOS6507_STATUS_FLAG_ZERO, !(tmp & 0xFF));
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
