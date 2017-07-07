/*
 * File: mos6507.c
 * Author: dgrubb
 * Date: 06/23/2017
 *
 * Provides implementations of the 6507 model.
 */

#include "mos6507.h"
#include "Atari-memmap.h"

/* Representation of our CPU */
static mos6507 cpu;

/* Invoking this function causes the state of the CPU to update
 * as if receiving an external clock tick. Note that the 6507
 * required at least two clock cycles to execute an opcode, usually
 * more depending on the memory addressing mode invoked.
 */
void mos6507_clock()
{
    /* If the CPU is still in the middle of decoing/executing an
     * operation then continue execution. Otherwise, read the next 
     * opcode out of memory and beging decode.
     */
    if (!cpu.current_instruction) {
        memmap_read(&cpu.current_instruction);
    }
    if(0 == opcode_execute(cpu.current_instruction)) {
        cpu.current_instruction = 0;
    }
}

void mos6507_reset()
{
    uint8_t pch, pcl = 0;
    uint16_t pc = 0;

    /* Clear model representations */
    mos6507_init();

    /* Now proceed through the 6507's regular startup sequence.
     * 1. Jump to reset vector 0xFFFC, read that byte as the 
     *    high byte of a memory address.
     * 2. Increment to 0xFFFD, read that byte as the low byte
     *    of a memory address.
     */
    mos6507_set_address_bus(0xFFFC);
    memmap_read(&pch);
    mos6507_set_address_bus(0xFFFD);
    memmap_read(&pcl);

    /* Pack the two bytes found at the reset vector into 
     * the program counter and initialise it as the true
     * start address of our program.
     */
    pc |= (pch << 8);
    pc |= pcl;
    mos6507_set_PC(pc);
    mos6507_set_address_bus(mos6507_get_PC());
}

void mos6507_init()
{
    /* Initialise all members back to 0 */
    mos6507 cpu = {0};
}

void mos6507_set_register(mos6507_register_t reg, uint8_t value)
{
    switch(reg) {
        case MOS6507_REG_A:  cpu.A = value;  break;
        case MOS6507_REG_Y:  cpu.Y = value;  break;
        case MOS6507_REG_X:  cpu.X = value;  break;
        case MOS6507_REG_PC: cpu.PC = value; break;
        case MOS6507_REG_S:  cpu.S = value;  break;
        case MOS6507_REG_P:  cpu.P = value;  break;
        default: /* Handle error */ break;
    }
}

void mos6507_get_register(mos6507_register_t reg, uint8_t *value)
{
    switch(reg) {
        case MOS6507_REG_A:  *value = cpu.A;  break;
        case MOS6507_REG_Y:  *value = cpu.Y;  break;
        case MOS6507_REG_X:  *value = cpu.X;  break;
        case MOS6507_REG_PC: *value = cpu.PC; break;
        case MOS6507_REG_S:  *value = cpu.S;  break;
        case MOS6507_REG_P:  *value = cpu.P;  break;
        default: /* Handle error */ break;
    }
}

void mos6507_increment_PC()
{
    cpu.PC++;
}

uint16_t mos6507_get_PC()
{
    return cpu.PC;
}

void mos6507_set_PC(uint16_t pc)
{
    cpu.PC = pc;
}

void mos6507_set_address_bus_hl(uint8_t adh, uint8_t adl)
{
    cpu.address_bus  = 0;
    cpu.address_bus |= (adh << 8);
    cpu.address_bus |= adl;
}

void mos6507_set_address_bus(uint16_t address)
{
    cpu.address_bus = address;
}

void mos6507_get_address_bus(uint16_t *address)
{
    *address = cpu.address_bus;
}

void mos6507_set_data_bus(uint8_t data)
{
    cpu.data_bus = data;
}

void mos6507_get_data_bus(uint8_t *data)
{
    *data = cpu.data_bus;
}
