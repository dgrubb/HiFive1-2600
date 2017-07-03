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
#include "mos6507-addressing-macros.h"
#include "Atari-memmap.h"

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

    /* Now fill out the actual codes supported by a real 6507 */

    /* 0x00: BRK, Implied */
    ISA_table[0x00].opcode = opcode_BRK;
    ISA_table[0x00].addressing_mode = OPCODE_ADDRESSING_MODE_IMPLIED;

    /* Load accumulator with memory */
    ISA_table[0xA9].opcode = opcode_LDA;
    ISA_table[0xA9].addressing_mode = OPCODE_ADDRESSING_MODE_IMMEDIATE;
    ISA_table[0xA5].opcode = opcode_LDA;
    ISA_table[0xA5].addressing_mode = OPCODE_ADDRESSING_MODE_ZERO_PAGE;
    ISA_table[0xB5].opcode = opcode_LDA;
    ISA_table[0xB5].addressing_mode = OPCODE_ADDRESSING_MODE_ZERO_PAGE_X_INDEXED;
    ISA_table[0xAD].opcode = opcode_LDA;
    ISA_table[0xAD].addressing_mode = OPCODE_ADDRESSING_MODE_ABSOLUTE;
    ISA_table[0xBD].opcode = opcode_LDA;
    ISA_table[0xBD].addressing_mode = OPCODE_ADDRESSING_MODE_ABSOLUTE_X_INDEXED;
    ISA_table[0xB9].opcode = opcode_LDA;
    ISA_table[0xB9].addressing_mode = OPCODE_ADDRESSING_MODE_ABSOLUTE_Y_INDEXED;
    ISA_table[0xA1].opcode = opcode_LDA;
    ISA_table[0xA1].addressing_mode = OPCODE_ADDRESSING_MODE_INDIRECT_X_INDEXED;
    ISA_table[0xB1].opcode = opcode_LDA;
    ISA_table[0xB1].addressing_mode = OPCODE_ADDRESSING_MODE_INDIRECT_Y_INDEXED;

    /* Load Index X with memory */
    ISA_table[0xA2].opcode = opcode_LDX;
    ISA_table[0xA2].addressing_mode = OPCODE_ADDRESSING_MODE_IMMEDIATE;
    ISA_table[0xA6].opcode = opcode_LDX;
    ISA_table[0xA6].addressing_mode = OPCODE_ADDRESSING_MODE_ZERO_PAGE;
    ISA_table[0xB6].opcode = opcode_LDX;
    ISA_table[0xB6].addressing_mode = OPCODE_ADDRESSING_MODE_ZERO_PAGE_Y_INDEXED;
    ISA_table[0xAE].opcode = opcode_LDX;
    ISA_table[0xAE].addressing_mode = OPCODE_ADDRESSING_MODE_ABSOLUTE;
    ISA_table[0xBE].opcode = opcode_LDX;
    ISA_table[0xBE].addressing_mode = OPCODE_ADDRESSING_MODE_ABSOLUTE_Y_INDEXED;

    /* Load Index Y with memory */
    ISA_table[0xA0].opcode = opcode_LDY;
    ISA_table[0xA0].addressing_mode = OPCODE_ADDRESSING_MODE_IMMEDIATE;
    ISA_table[0xA4].opcode = opcode_LDY;
    ISA_table[0xA4].addressing_mode = OPCODE_ADDRESSING_MODE_ZERO_PAGE;
    ISA_table[0xB4].opcode = opcode_LDY;
    ISA_table[0xB4].addressing_mode = OPCODE_ADDRESSING_MODE_ZERO_PAGE_X_INDEXED;
    ISA_table[0xAC].opcode = opcode_LDY;
    ISA_table[0xAC].addressing_mode = OPCODE_ADDRESSING_MODE_ABSOLUTE;
    ISA_table[0xBC].opcode = opcode_LDY;
    ISA_table[0xBB].addressing_mode = OPCODE_ADDRESSING_MODE_ABSOLUTE_X_INDEXED;

    /* Store Accumulator in memory */
    ISA_table[0x85].opcode = opcode_STA;
    ISA_table[0x85].addressing_mode = OPCODE_ADDRESSING_MODE_ZERO_PAGE;
    ISA_table[0x95].opcode = opcode_STA;
    ISA_table[0x95].addressing_mode = OPCODE_ADDRESSING_MODE_ZERO_PAGE_X_INDEXED;
    ISA_table[0x8D].opcode = opcode_STA;
    ISA_table[0x8D].addressing_mode = OPCODE_ADDRESSING_MODE_ABSOLUTE;
    ISA_table[0x9D].opcode = opcode_STA;
    ISA_table[0x9D].addressing_mode = OPCODE_ADDRESSING_MODE_ABSOLUTE_X_INDEXED;
    ISA_table[0x99].opcode = opcode_STA;
    ISA_table[0x99].addressing_mode = OPCODE_ADDRESSING_MODE_ABSOLUTE_Y_INDEXED;
    ISA_table[0x81].opcode = opcode_STA;
    ISA_table[0x81].addressing_mode = OPCODE_ADDRESSING_MODE_INDIRECT_X_INDEXED;
    ISA_table[0x91].opcode = opcode_STA;
    ISA_table[0x91].addressing_mode = OPCODE_ADDRESSING_MODE_INDIRECT_Y_INDEXED;

    /* Store Index X in memory */
    ISA_table[0x86].opcode = opcode_STX;
    ISA_table[0x86].addressing_mode = OPCODE_ADDRESSING_MODE_ZERO_PAGE;
    ISA_table[0x96].opcode = opcode_STX;
    ISA_table[0x96].addressing_mode = OPCODE_ADDRESSING_MODE_ZERO_PAGE_Y_INDEXED;
    ISA_table[0x8E].opcode = opcode_STX;
    ISA_table[0x8E].addressing_mode = OPCODE_ADDRESSING_MODE_ABSOLUTE;

    /* Store Index Y in memory */
    ISA_table[0x84].opcode = opcode_STY;
    ISA_table[0x84].addressing_mode = OPCODE_ADDRESSING_MODE_ZERO_PAGE;
    ISA_table[0x94].opcode = opcode_STY;
    ISA_table[0x94].addressing_mode = OPCODE_ADDRESSING_MODE_ZERO_PAGE_X_INDEXED;
    ISA_table[0x8C].opcode = opcode_STY;
    ISA_table[0x8C].addressing_mode = OPCODE_ADDRESSING_MODE_ABSOLUTE;

    /* Add memory to Accumulator with carry */
    ISA_table[0x69].opcode = opcode_ADC;
    ISA_table[0x69].addressing_mode = OPCODE_ADDRESSING_MODE_IMMEDIATE;
    ISA_table[0x65].opcode = opcode_ADC;
    ISA_table[0x65].addressing_mode = OPCODE_ADDRESSING_MODE_ZERO_PAGE;
    ISA_table[0x75].opcode = opcode_ADC;
    ISA_table[0x75].addressing_mode = OPCODE_ADDRESSING_MODE_ZERO_PAGE_X_INDEXED;
    ISA_table[0x6D].opcode = opcode_ADC;
    ISA_table[0x6D].addressing_mode = OPCODE_ADDRESSING_MODE_ABSOLUTE;
    ISA_table[0x7D].opcode = opcode_ADC;
    ISA_table[0x7D].addressing_mode = OPCODE_ADDRESSING_MODE_ABSOLUTE_X_INDEXED;
    ISA_table[0x79].opcode = opcode_ADC;
    ISA_table[0x79].addressing_mode = OPCODE_ADDRESSING_MODE_ABSOLUTE_Y_INDEXED;
    ISA_table[0x61].opcode = opcode_ADC;
    ISA_table[0x61].addressing_mode = OPCODE_ADDRESSING_MODE_INDIRECT_X_INDEXED;
    ISA_table[0x71].opcode = opcode_ADC;
    ISA_table[0x71].addressing_mode = OPCODE_ADDRESSING_MODE_INDIRECT_Y_INDEXED;

    /* Subtract memory from Accumulator with borrow */
    ISA_table[0xE9].opcode = opcode_SBC;
    ISA_table[0xE9].addressing_mode = OPCODE_ADDRESSING_MODE_IMMEDIATE;
    ISA_table[0xE5].opcode = opcode_SBC;
    ISA_table[0xE5].addressing_mode = OPCODE_ADDRESSING_MODE_ZERO_PAGE;
    ISA_table[0xF5].opcode = opcode_SBC;
    ISA_table[0xF5].addressing_mode = OPCODE_ADDRESSING_MODE_ZERO_PAGE_X_INDEXED;
    ISA_table[0xED].opcode = opcode_SBC;
    ISA_table[0xED].addressing_mode = OPCODE_ADDRESSING_MODE_ABSOLUTE;
    ISA_table[0xFD].opcode = opcode_SBC;
    ISA_table[0xFD].addressing_mode = OPCODE_ADDRESSING_MODE_ABSOLUTE_X_INDEXED;
    ISA_table[0xF9].opcode = opcode_SBC;
    ISA_table[0xF9].addressing_mode = OPCODE_ADDRESSING_MODE_ABSOLUTE_Y_INDEXED;
    ISA_table[0xE1].opcode = opcode_SBC;
    ISA_table[0xE1].addressing_mode = OPCODE_ADDRESSING_MODE_INDIRECT_X_INDEXED;
    ISA_table[0xF1].opcode = opcode_SBC;
    ISA_table[0xF1].addressing_mode = OPCODE_ADDRESSING_MODE_INDIRECT_Y_INDEXED;

    /* Increment memory by one */
    ISA_table[0xE6].opcode = opcode_INC;
    ISA_table[0xE6].addressing_mode = OPCODE_ADDRESSING_MODE_ZERO_PAGE;
    ISA_table[0xF6].opcode = opcode_INC;
    ISA_table[0xF6].addressing_mode = OPCODE_ADDRESSING_MODE_ZERO_PAGE_X_INDEXED;
    ISA_table[0xEE].opcode = opcode_INC;
    ISA_table[0xEE].addressing_mode = OPCODE_ADDRESSING_MODE_ABSOLUTE;
    ISA_table[0xFE].opcode = opcode_INC;
    ISA_table[0xFE].addressing_mode = OPCODE_ADDRESSING_MODE_ABSOLUTE_X_INDEXED;

    /* Increment Index X by one */
    ISA_table[0xE8].opcode = opcode_INX;
    ISA_table[0xE8].addressing_mode = OPCODE_ADDRESSING_MODE_IMPLIED;

    /* Increment Index Y by one */
    ISA_table[0xC8].opcode = opcode_INY;
    ISA_table[0xC8].addressing_mode = OPCODE_ADDRESSING_MODE_IMPLIED;

    /* Decrement memory by one */
    ISA_table[0xC6].opcode = opcode_DEC;
    ISA_table[0xC6].addressing_mode = OPCODE_ADDRESSING_MODE_ZERO_PAGE;
    ISA_table[0xD6].opcode = opcode_DEC;
    ISA_table[0xD6].addressing_mode = OPCODE_ADDRESSING_MODE_ZERO_PAGE_X_INDEXED;
    ISA_table[0xCE].opcode = opcode_DEC;
    ISA_table[0xCE].addressing_mode = OPCODE_ADDRESSING_MODE_ABSOLUTE;
    ISA_table[0xDE].opcode = opcode_DEC;
    ISA_table[0xDE].addressing_mode = OPCODE_ADDRESSING_MODE_ABSOLUTE_X_INDEXED;

    /* Decrement Index X by one */
    ISA_table[0xCA].opcode = opcode_DEX;
    ISA_table[0xCA].addressing_mode = OPCODE_ADDRESSING_MODE_IMPLIED;

    /* Decrement Index Y by one */
    ISA_table[0x88].opcode = opcode_DEY;
    ISA_table[0x88].addressing_mode = OPCODE_ADDRESSING_MODE_IMPLIED;
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
    static uint8_t adl, adh, bal, bah, data = 0;
    uint8_t X, Y, c = 0;

    FETCH_DATA()
    mos6507_ADC(data);
    END_OPCODE()
    return 0;
}

int opcode_AND(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bal, bah, data = 0;
    uint8_t X, Y, c = 0;

    FETCH_DATA()
    mos6507_AND(data);
    END_OPCODE()
    return 0;
}

int opcode_ASL(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    if (OPCODE_ADDRESSING_MODE_ACCUMULATOR == address_mode) {
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
        END_OPCODE()
        return 0;
    }

    FETCH_DATA()
    mos6507_ASL(data);
    END_OPCODE()
    return 0;
}

int opcode_BCC(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_BCS(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_BEQ(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_BIT(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    FETCH_DATA()
    mos6507_BIT(data);
    END_OPCODE()
    return 0;
}

int opcode_BMI(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_BNE(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_BPL(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_BRK(int cycle, addressing_mode_t address_mode)
{
}

int opcode_BVC(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_BVS(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_CLC(int cycle, addressing_mode_t address_mode)
{
    uint8_t value = 0;
    switch(cycle) {
        case 0:
            /* Consume clock cycle for fetching op-code */
            return -1;
        case 1:
            mos6507_get_register(MOS6507_REG_P, &value);
            value &= ~(MOS6507_STATUS_FLAG_CARRY);
            mos6507_set_register(MOS6507_REG_P, value);
            /* Intentional fall-through */
        default:
            /* End of op-code execution */
            break;
    }

    END_OPCODE()
    return 0;
}

int opcode_CLD(int cycle, addressing_mode_t address_mode)
{
    uint8_t value = 0;
    switch(cycle) {
        case 0:
            /* Consume clock cycle for fetching op-code */
            return -1;
        case 1:
            mos6507_get_register(MOS6507_REG_P, &value);
            value &= ~(MOS6507_STATUS_FLAG_DECIMAL);
            mos6507_set_register(MOS6507_REG_P, value);
            /* Intentional fall-through */
        default:
            /* End of op-code execution */
            break;
    }

    END_OPCODE()
    return 0;
}

int opcode_CLI(int cycle, addressing_mode_t address_mode)
{
    uint8_t value = 0;
    switch(cycle) {
        case 0:
            /* Consume clock cycle for fetching op-code */
            return -1;
        case 1:
            mos6507_get_register(MOS6507_REG_P, &value);
            value &= ~(MOS6507_STATUS_FLAG_INTERRUPT);
            mos6507_set_register(MOS6507_REG_P, value);
            /* Intentional fall-through */
        default:
            /* End of op-code execution */
            break;
    }

    END_OPCODE()
    return 0;
}

int opcode_CLV(int cycle, addressing_mode_t address_mode)
{
    uint8_t value = 0;
    switch(cycle) {
        case 0:
            /* Consume clock cycle for fetching op-code */
            return -1;
        case 1:
            mos6507_get_register(MOS6507_REG_P, &value);
            value &= ~(MOS6507_STATUS_FLAG_OVERFLOW);
            mos6507_set_register(MOS6507_REG_P, value);
            /* Intentional fall-through */
        default:
            /* End of op-code execution */
            break;
    }

    END_OPCODE()
    return 0;
}

int opcode_CMP(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bal, bah, data = 0;
    uint8_t X, Y, c = 0;

    FETCH_DATA()
    mos6507_CMP(data);
    END_OPCODE()
    return 0;
}

int opcode_CPX(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    FETCH_DATA()
    mos6507_CPX(data);
    END_OPCODE()
    return 0;
}

int opcode_CPY(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    FETCH_DATA()
    mos6507_CPY(data);
    END_OPCODE()
    return 0;
}

int opcode_DEC(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_DEX(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_DEY(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_EOR(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_INC(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_INX(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_INY(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_JMP(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_JSR(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_LDA(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_LDX(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_LDY(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_LSR(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_NOP(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
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

int opcode_PHA(int cycle, addressing_mode_t address_mode)
{
    uint8_t value, destination = 0;
    switch(cycle) {
        case 0:
            /* Consume clock cycle for fetching op-code */
            return -1;
        case 1:
            /* Consume another clock cycle incrementing PC */
            mos6507_increment_PC();
            return -1;
        case 2:
            /* Fetch value of status register and stack pointer */
            mos6507_get_register(MOS6507_REG_A, &value);
            mos6507_get_register(MOS6507_REG_S, &destination);
            /* Write status register value to stack address */
            mos6507_set_address_bus_hl(0, destination);
            mos6507_set_data_bus(value);
            memmap_write();
            /* Intentional fall-through */
        default:
            /* End of op-code execution */
            break;
    }
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
            return -1;
        case 2:
            /* Fetch value of status register and stack pointer */
            mos6507_get_register(MOS6507_REG_P, &value);
            mos6507_get_register(MOS6507_REG_S, &destination);
            /* Write status register value to stack address */
            mos6507_set_address_bus_hl(0, destination);
            mos6507_set_data_bus(value);
            memmap_write();
            /* Intentional fall-through */
        default:
            /* End of op-code execution */
            break;
    }
    END_OPCODE();
    return 0;
}

int opcode_PLA(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_ROL(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_ROR(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_RTI(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_RTS(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_SBC(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_SEC(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_SED(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_SEI(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_STA(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_STX(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_STY(int cycle, addressing_mode_t address_mode)
{
    static uint8_t adl, adh, bah, bal, data = 0;
    uint8_t X, Y, c = 0;

    END_OPCODE()
    return 0;
}

int opcode_TAX(int cycle, addressing_mode_t address_mode)
{
    uint8_t value = 0;
    switch(cycle) {
        case 0:
            /* Consume clock cycle for fetching op-code */
            return -1;
        case 1:
            mos6507_get_register(MOS6507_REG_A, &value);
            mos6507_set_register(MOS6507_REG_X, value);
            /* Intentional fall-through */
        default:
            /* End of op-code execution */
            break;
    }

    END_OPCODE()
    return 0;
}

int opcode_TAY(int cycle, addressing_mode_t address_mode)
{
    uint8_t value = 0;
    switch(cycle) {
        case 0:
            /* Consume clock cycle for fetching op-code */
            return -1;
        case 1:
            mos6507_get_register(MOS6507_REG_A, &value);
            mos6507_set_register(MOS6507_REG_Y, value);
            /* Intentional fall-through */
        default:
            /* End of op-code execution */
            break;
    }

    END_OPCODE()
    return 0;
}

int opcode_TSX(int cycle, addressing_mode_t address_mode)
{
    uint8_t value = 0;
    switch(cycle) {
        case 0:
            /* Consume clock cycle for fetching op-code */
            return -1;
        case 1:
            mos6507_get_register(MOS6507_REG_P, &value);
            mos6507_set_register(MOS6507_REG_X, value);
            /* Intentional fall-through */
        default:
            /* End of op-code execution */
            break;
    }

    END_OPCODE()
    return 0;
}

int opcode_TXA(int cycle, addressing_mode_t address_mode)
{
    uint8_t value = 0;
    switch(cycle) {
        case 0:
            /* Consume clock cycle for fetching op-code */
            return -1;
        case 1:
            mos6507_get_register(MOS6507_REG_X, &value);
            mos6507_set_register(MOS6507_REG_A, value);
            /* Intentional fall-through */
        default:
            /* End of op-code execution */
            break;
    }

    END_OPCODE()
    return 0;
}

int opcode_TXS(int cycle, addressing_mode_t address_mode)
{
    uint8_t value = 0;
    switch(cycle) {
        case 0:
            /* Consume clock cycle for fetching op-code */
            return -1;
        case 1:
            mos6507_get_register(MOS6507_REG_X, &value);
            mos6507_set_register(MOS6507_REG_P, value);
            /* Intentional fall-through */
        default:
            /* End of op-code execution */
            break;
    }

    END_OPCODE()
    return 0;
}

int opcode_TYA(int cycle, addressing_mode_t address_mode)
{
    uint8_t value = 0;
    switch(cycle) {
        case 0:
            /* Consume clock cycle for fetching op-code */
            return -1;
        case 1:
            mos6507_get_register(MOS6507_REG_Y, &value);
            mos6507_set_register(MOS6507_REG_A, value);
            /* Intentional fall-through */
        default:
            /* End of op-code execution */
            break;
    }

    END_OPCODE()
    return 0;
}

