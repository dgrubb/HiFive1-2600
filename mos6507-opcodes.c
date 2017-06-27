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
void opcode_execute(uint8_t opcode)
{
    ISA_table[opcode].opcode(
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
        ISA_table[i].addressing_mode = OPCODE_ADDRESSING_MODE_IMPLIED;
    }

    /* Now fill out the actual codes supported by a real 6507.
     * Annotation format:
     * [Hex code]:[Instruction], [Addressing mode]
     */

    /* 0x00: BRK, Implied*/
    ISA_table[0x00].opcode = opcode_BRK;
    ISA_table[0x00].addressing_mode = OPCODE_ADDRESSING_MODE_IMPLIED;

    /* 0x01: ORA, X indexed indirect */
    ISA_table[0x01].opcode = opcode_ORA;
    ISA_table[0x01].addressing_mode = OPCODE_ADDRESSING_MODE_X_INDEXED_INDIRECT;

    /* 0x05: ORA, Zero-page */
    ISA_table[0x05].opcode = opcode_ORA;
    ISA_table[0x05].addressing_mode = OPCODE_ADDRESSING_MODE_ZERO_PAGE;

    /* 0x06: ASL, Zero-page */
    ISA_table[0x06].opcode = opcode_ASL;
    ISA_table[0x06].addressing_mode = OPCODE_ADDRESSING_MODE_ZERO_PAGE;

    /* 0x08: PHP, Implied */
    ISA_table[0x08].opcode = opcode_PHP;
    ISA_table[0x08].addressing_mode = OPCODE_ADDRESSING_MODE_IMPLIED;

    /* 0x09: ORA, Immediate */
    ISA_table[0x09].opcode = opcode_ORA;
    ISA_table[0x09].addressing_mode = OPCODE_ADDRESSING_MODE_IMMEDIATE;

    /* 0x0A: ASL, Accumulator */
    ISA_table[0x0A].opcode = opcode_ASL;
    ISA_table[0x0A].addressing_mode = OPCODE_ADDRESSING_MODE_ACCUMULATOR;

    /* 0x0D: ORA, Absolute */
    ISA_table[0x0D].opcode = opcode_ORA;
    ISA_table[0x0D].addressing_mode = OPCODE_ADDRESSING_MODE_ABSOLUTE;

    /* 0x0E: ASL, Absolute */
    ISA_table[0x0E].opcode = opcode_ASL;
    ISA_table[0x0E].addressing_mode = OPCODE_ADDRESSING_MODE_ABSOLUTE;

    /* 0x10: BPL, Relative */
    ISA_table[0x10].opcode = opcode_BPL;
    ISA_table[0x10].addressing_mode = OPCODE_ADDRESSING_MODE_RELATIVE;

    /* 0x11: ORA, Indirect Y indexed */
    ISA_table[0x11].opcode = opcode_ORA;
    ISA_table[0x11].addressing_mode = OPCODE_ADDRESSING_MODE_INDIRECT_Y_INDEXED;

    /* 0x15: ORA, Zero-page X indexed */
    ISA_table[0x15].opcode = opcode_ORA;
    ISA_table[0x15].addressing_mode = OPCODE_ADDRESSING_MODE_ZERO_PAGE_X_INDEXED;

    /* 0x16: ASL, Zero-page X indexed */
    ISA_table[0x16].opcode = opcode_ASL;
    ISA_table[0x16].addressing_mode = OPCODE_ADDRESSING_MODE_ZERO_PAGE_X_INDEXED;

    /* 0x18: CLC, Implied */
    ISA_table[0x18].opcode = opcode_CLC;
    ISA_table[0x18].addressing_mode = OPCODE_ADDRESSING_MODE_IMPLIED;

    /* 0x19: ORA, Absolute Y indexed */
    ISA_table[0x19].opcode = opcode_ORA;
    ISA_table[0x19].addressing_mode = OPCODE_ADDRESSING_MODE_ABSOLUTE_Y_INDEXED;
}

/******************************************************************************
 * Instruction set implementation
 *****************************************************************************/

void opcode_ILL(addressing_mode_t address_mode)
{
    
}

void opcode_ASL(addressing_mode_t address_mode)
{
}

void opcode_BPL(addressing_mode_t address_mode)
{
}

void opcode_BRK(addressing_mode_t address_mode)
{
}

void opcode_CLC(addressing_mode_t address_mode)
{
}

void opcode_ORA(addressing_mode_t address_mode)
{
}

void opcode_PHP(addressing_mode_t address_mode)
{
    static int cycle = 0;
    uint8_t value = 0;
    uint8_t destination = 0;
    switch(cycle) {
        case 0:
            /* Consume clock cycle for fetching op-code */
            cycle++;
            return;
        case 1:
            /* Consume another clock cycle incrementing PC */
            mos6507_increment_PC();
            cycle++;
            return;
        case 2:
            /* Fetch value of status register and stack pointer */
            mos6507_get_register(MOS6507_REG_P, &value);
            mos6507_get_register(MOS6507_REG_S, &destination);
            /* Write status register value to stack address */
            mos6532_write(value, destination);
        default:
            /* End of op-code execution */
            break;
    }
    /* Reset cycle count for next invocation of this op-code */
    cycle = 0;
}

