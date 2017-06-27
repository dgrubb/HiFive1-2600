/*
 * File: mos6507-opcodes.c
 * Author: dgrubb
 * Date: 12/20/2016
 *
 * Provides implementations of the 6507's op-codes.
 *
 * Useful information about cycle execution times:
 * http://users.telenet.be/kim1-6502/6502/hwman.html##AA
 */

#include "mos6507.h"
#include "mos6507-opcodes.h"
#include "mos6507-microcode.h"
#include "mos6532.h"

/* Invoked at the end of each op-code. Resets the
 * clock cycle counter and increments the PC and 
 * address busses for the next op-code
 */
#define END_OPCODE() \
    mos6507_increment_PC(); \
    mos6507_set_address_bus(mos6507_get_PC());

instruction_t ISA_table[ISA_LENGTH];

/* Looks up an instruction from the instruction table and
 * executes the corresponding function, passing along cycle
 * time and addressing mode.
 *
 * opcode: value of the instruction, e.g., 0x00 for BRK
 */
int opcode_execute(uint8_t opcode)
{
    static int cycle = 0;
    if (-1 == ISA_table[opcode].opcode(cycle, ISA_table[opcode].addressing_mode)) {
        cycle++;
    } else {
        cycle = 0;
    }
    return cycle;
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
    ISA_table[0x01].addressing_mode = OPCODE_ADDRESSING_MODE_INDIRECT_X_INDEXED;

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
 *
 * These functions exist to wrapper the logical instruction (e.g., ADD, ROR,
 * MOV etc) with the necessary clock steps and memory access methods to make
 * the emulation cycle accurate. Actual op-code logic is implemented in
 * mos6507-microcode.c which has no knowledge of the memory map beyond what is
 * actually present in the CPU itself.
 *****************************************************************************/

int opcode_ILL(int cycle, addressing_mode_t address_mode)
{
    
}

int opcode_ADC(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, zp_adl, zp_adh, bal, bah, data = 0;
    uint8_t X = 0;

    if (OPCODE_ADDRESSING_MODE_IMMEDIATE == address_mode) {
        switch(cycle) {
            case 0:
                /* Consume clock cycle for fetching op-code */
                return -1;
            case 1:
                mos6507_increment_PC();
                mos6507_set_address_bus(mos6507_get_PC());
                mos6532_read(&data);
                mos6507_ADC(data);
                /* Intentional fall-through */
            default:
                /* End of op-code execution */
                break;
        }
    } else if (OPCODE_ADDRESSING_MODE_ZERO_PAGE == address_mode) {
        switch(cycle) {
            case 0:
                /* Consume clock cycle for fetching op-code */
                return -1;
            case 1:
                mos6507_increment_PC();
                mos6507_set_address_bus(mos6507_get_PC());
                mos6532_read(&zp_adl);
                return -1;
            case 2:
                mos6507_set_address_bus_hl(0, zp_adl);
                mos6532_read(&data);
                mos6507_ADC(data);
                /* Intentional fall-through */
            default:
                /* End of op-code execution */
                break;
        }
    } else if (OPCODE_ADDRESSING_MODE_ABSOLUTE == address_mode) {
        switch(cycle) {
            case 0:
                /* Consume clock cycle for fetching op-code */
                return -1;
            case 1:
                mos6507_increment_PC();
                mos6507_set_address_bus(mos6507_get_PC());
                mos6532_read(&zp_adl);
                return -1;
            case 2:
                mos6507_increment_PC();
                mos6507_set_address_bus(mos6507_get_PC());
                mos6532_read(&zp_adh);
                return -1;
            case 3:
                mos6507_set_address_bus_hl(zp_adh, zp_adl);
                mos6532_read(&data);
                mos6507_ADC(data);
                /* Intentional fall-through */
            default:
                /* End of op-code execution */
                break;
        }
    } else if (OPCODE_ADDRESSING_MODE_INDIRECT_X_INDEXED == address_mode) {
        switch(cycle) {
            case 0:
                /* Consume clock cycle for fetching op-code */
                return -1;
            case 1:
                mos6507_increment_PC();
                mos6532_read(&bal);
                return -1;
            case 2:
                mos6507_set_address_bus_hl(0, bal);
                return -1;
            case 3:
                mos6507_get_register(MOS6507_REG_X, &X);
                mos6507_set_address_bus_hl(0, (bal + X));
                /* TODO: complete this call */
                return -1;
                /* Intentional fall-through */
            default:
                /* End of op-code execution */
                break;
        }
    };
    END_OPCODE();
    return 0;
}

int opcode_ASL(int cycle, addressing_mode_t address_mode)
{
}

int opcode_BPL(int cycle, addressing_mode_t address_mode)
{
}

int opcode_BRK(int cycle, addressing_mode_t address_mode)
{
}

int opcode_CLC(int cycle, addressing_mode_t address_mode)
{
}

int opcode_ORA(int cycle, addressing_mode_t address_mode)
{
    uint8_t accumulator, data, result, status = 0;
    if (OPCODE_ADDRESSING_MODE_IMMEDIATE == address_mode) {
        switch(cycle) {
            case 0:
                /* Consume clock cycle for fetching op-code */
                return -1;
            case 1: ;
                /* Intentional fall-through */
            default:
                /* End of op-code execution */
                break;
        }
    } else if (OPCODE_ADDRESSING_MODE_ZERO_PAGE == address_mode) {
    };
    END_OPCODE();
    return 0;
}

int opcode_PHP(int cycle, addressing_mode_t address_mode)
{
    uint8_t value, destination = 0;
    switch(cycle) {
        case 0:
            /* Consume clock cycle for fetching op-code */
            return -1;
        case 1:
            /* Consume another clock cycle incrementing PC */
            mos6507_increment_PC();
            cycle++;
            return -1;
        case 2:
            /* Fetch value of status register and stack pointer */
            mos6507_get_register(MOS6507_REG_P, &value);
            mos6507_get_register(MOS6507_REG_S, &destination);
            /* Write status register value to stack address */
            mos6507_set_address_bus_hl(0, destination);
            mos6507_set_data_bus(value);
            mos6532_write();
            /* Intentional fall-through */
        default:
            /* End of op-code execution */
            break;
    }
    END_OPCODE();
    return 0;
}

