/*
 * File: mos6507-opcodes.c
 * Author: dgrubb
 * Date: 12/20/2016
 *
 * Provides implementations of the 6507's op-codes.
 */

#include "mos6507.h"
#include "mos6507-opcodes.h"
#include "mos6532.h"

instruction_t ISA_table[ISA_LENGTH];

/* Looks up an instruction from the instruction table and
 * executes the corresponding function, passing along cycle
 * time and addressing mode.
 *
 * opcode: value of the instruction, e.g., 0x00 for BRK
 */
void opcode_execute(char opcode)
{
    ISA_table[opcode].opcode(
        ISA_table[opcode].cycles,
        ISA_table[opcode].addressing_mode
    );
}

/* Loads an array with function pointers to the corresponding
 * instruction. E.g., LDY is 0x0C so index 12 of the ISA table
 * would be loaded with a pointer to function opcode_LDY().
 */
void opcode_populate_ISA_table()
{
    int i = 0;
    /* Initially, fill the entire table with ILL */
    for (i=0; i<ISA_LENGTH; i++) {
        ISA_table[i].opcode = opcode_ILL;
        ISA_table[i].cycles = 1;
        ISA_table[i].addressing_mode = OPCODE_ADDRESSING_MODE_IMPLIED;
    }

    /* Now fill out the actual codes supported by a real 6507.
     * Annotation format:
     * [Hex code]:[Instruction], [Addressing mode]
     */

    /* 0x00: BRK, Implied*/
    ISA_table[0x00].opcode = opcode_BRK;
    ISA_table[0x00].cycles = 7;
    ISA_table[0x00].addressing_mode = OPCODE_ADDRESSING_MODE_IMPLIED;

    /* 0x01: ORA, X indexed indirect */
    ISA_table[0x01].opcode = opcode_ORA;
    ISA_table[0x01].cycles = 6;
    ISA_table[0x01].addressing_mode = OPCODE_ADDRESSING_MODE_X_INDEXED_INDIRECT;

    /* 0x05: ORA, Zero-page */
    ISA_table[0x05].opcode = opcode_ORA;
    ISA_table[0x05].cycles = 3;
    ISA_table[0x05].addressing_mode = OPCODE_ADDRESSING_MODE_ZERO_PAGE;

    /* 0x06: ASL, Zero-page */
    ISA_table[0x06].opcode = opcode_ASL;
    ISA_table[0x06].cycles = 6;
    ISA_table[0x06].addressing_mode = OPCODE_ADDRESSING_MODE_ZERO_PAGE;

    /* 0x08: PHP, Implied */
    ISA_table[0x08].opcode = opcode_PHP;
    ISA_table[0x08].cycles = 4;
    ISA_table[0x08].addressing_mode = OPCODE_ADDRESSING_MODE_IMPLIED;

    /* 0x09: ORA, Immediate */
    ISA_table[0x09].opcode = opcode_ORA;
    ISA_table[0x09].cycles = 2;
    ISA_table[0x09].addressing_mode = OPCODE_ADDRESSING_MODE_IMMEDIATE;

    /* 0x0A: ASL, Accumulator */
    ISA_table[0x0A].opcode = opcode_ASL;
    ISA_table[0x0A].cycles = 2;
    ISA_table[0x0A].addressing_mode = OPCODE_ADDRESSING_MODE_ACCUMULATOR;

    /* 0x0D: ORA, Absolute */
    ISA_table[0x0D].opcode = opcode_ORA;
    ISA_table[0x0D].cycles = 4;
    ISA_table[0x0D].addressing_mode = OPCODE_ADDRESSING_MODE_ABSOLUTE;

    /* 0x0E: ASL, Absolute */
    ISA_table[0x0E].opcode = opcode_ASL;
    ISA_table[0x0E].cycles = 6;
    ISA_table[0x0E].addressing_mode = OPCODE_ADDRESSING_MODE_ABSOLUTE;

    /* 0x10: BPL, Relative */
    ISA_table[0x10].opcode = opcode_BPL;
    ISA_table[0x10].cycles = 2;
    ISA_table[0x10].addressing_mode = OPCODE_ADDRESSING_MODE_RELATIVE;

    /* 0x11: ORA, Indirect Y indexed */
    ISA_table[0x11].opcode = opcode_ORA;
    ISA_table[0x11].cycles = 5;
    ISA_table[0x11].addressing_mode = OPCODE_ADDRESSING_MODE_INDIRECT_Y_INDEXED;

    /* 0x15: ORA, Zero-page X indexed */
    ISA_table[0x15].opcode = opcode_ORA;
    ISA_table[0x15].cycles = 4;
    ISA_table[0x15].addressing_mode = OPCODE_ADDRESSING_MODE_ZERO_PAGE_X_INDEXED;

    /* 0x16: ASL, Zero-page X indexed */
    ISA_table[0x16].opcode = opcode_ASL;
    ISA_table[0x16].cycles = 6;
    ISA_table[0x16].addressing_mode = OPCODE_ADDRESSING_MODE_ZERO_PAGE_X_INDEXED;

    /* 0x18: CLC, Implied */
    ISA_table[0x18].opcode = opcode_CLC;
    ISA_table[0x18].cycles = 2;
    ISA_table[0x18].addressing_mode = OPCODE_ADDRESSING_MODE_IMPLIED;

    /* 0x19: ORA, Absolute Y indexed */
    ISA_table[0x19].opcode = opcode_ORA;
    ISA_table[0x19].cycles = 4;
    ISA_table[0x19].addressing_mode = OPCODE_ADDRESSING_MODE_ABSOLUTE_Y_INDEXED;
}

/******************************************************************************
 * Instruction set implementation
 *****************************************************************************/

void opcode_ILL(int cycles, addressing_mode_t address_mode)
{
    
}

void opcode_ASL(int cycle, addressing_mode_t address_mode)
{
}

void opcode_BPL(int cycle, addressing_mode_t address_mode)
{
}

void opcode_BRK(int cycles, addressing_mode_t address_mode)
{
}

void opcode_CLC(int cycle, addressing_mode_t address_mode)
{
}

void opcode_ORA(int cycle, addressing_mode_t address_mode)
{
}

void opcode_PHP(int cycles, addressing_mode_t address_mode)
{
    mos6532_write(cpu.S, cpu.P);
}

