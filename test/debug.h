/*
 * File: debug.h
 * Author: dgrubb
 * Date: 08/07/2017
 *
 * Provides helper functions for printing human-readable 
 * information.
 */

#ifndef _DEBUG_H
#define _DEBUG_H

#include "../cpu/mos6507.h"

typedef struct {
    uint8_t op;
    const char *str;
} debug_opcode_t;

debug_opcode_t debug_opcodes[] = {
    { 0x00, "None"},
    /* LDA */
    {0xA9, "LDA - Immediate"},
    {0xA5, "LDA - Zero page"},
    {0xB5, "LDA - Zero page X indexed"},
    {0xAD, "LDA - Absolute"},
    {0xBD, "LDA - Absolute X indexed"},
    {0xB9, "LDA - Absolute Y indexed"},
    {0xA1, "LDA - Indirect X indexed"},
    {0xB1, "LDA - Indirect Y indexed"},
    /* LDX */
    {0xA2, "LDX - Immediate"},
    {0xA6, "LDX - Zero page"},
    {0xB6, "LDX - Zero page Y indexed"},
    {0xAE, "LDX - Absolute"},
    {0xBE, "LDX - Absolute Y indexed"},
    /* LDY */
    {0xA0, "LDY - Immediate"},
    {0xA4, "LDY - Zero page"},
    {0xB4, "LDY - Zero page X indexed"},
    {0xAC, "LDY - Absolute"},
    {0xBC, "LDY - Absolute X indexed"},
    /* STA */
    {0x85, "STA - Zero page"},
    {0x95, "STA - Zero page X indexed"},
    {0x8D, "STA - Absolute"},
    {0x9D, "STA - Absolute X indexed"},
    {0x99, "STA - Absolute Y indexed"},
    {0x81, "STA - Indirect X indexed"},
    {0x91, "STA - Indirect Y indexed"},
    /* STX */
    {0x86, "STX - Zero page"},
    {0x96, "STX - Zero page Y indexed"},
    {0x8E, "STX - Absolute"},
    /* STY */
    {0x84, "STY - Zero page"},
    {0x94, "STY - Zero page Y indexed"},
    {0x8E, "STY - Absolute"},
    /* ADC */
    {0x69, "ADC - Immediate"},
    {0x65, "ADC - Zero page"},
    {0x75, "ADC - Zero page X indexed"},
    {0x6D, "ADC - Absolute"},
    {0x7D, "ADC - Absolute X indexed"},
    {0x79, "ADC - Absolute Y indexed"},
    {0x61, "ADC - Indirect X indexed"},
    {0x71, "ADC - Indirect Y indexed"},
    /* SBC */
    {0xE9, "SBC - Immediate"},
    {0xE5, "SBC - Zero page"},
    {0xF5, "SBC - Zero page X indexed"},
    {0xED, "SBC - Absolute"},
    {0xFD, "SBC - Absolute X indexed"},
    {0xF9, "SBC - Absolute Y indexed"},
    {0xE1, "SBC - Indirect X indexed"},
    {0xF1, "SBC - Indirect Y indexed"},
    /* INC */
    {0xE6, "INC - Zero page"},
    {0xF6, "INC - Zero page X indexed"},
    {0xEE, "INC - Absolute"},
    {0xFE, "INC - Absolute X indexed"},
    /* INX */
    {0xE8, "INX - Implied"},
    /* INY */
    {0xFE, "INY - Implied"},
    /* DEC */
    {0xC6, "DEC - Zero page"},
    {0xD6, "DEC - Zero page X indexed"},
    {0xCE, "DEC - Absolute"},
    {0xDE, "DEC - Absolute X indexed"},
    /* DEX */
    {0xCA, "DEX - Implied"},
    /* DEY */
    {0x88, "DEY - Implied"},
    /* JMP */
    {0x4C, "JMP - Absolute"},
    {0x6C, "JMP - Indirect"},
    /* BCC */
    {0x90, "BCC - Relative"},
    /* BCS */
    {0xB0, "BCS - Relative"},
    /* BEQ */
    {0xF0, "BEQ - Relative"},
    /* BNE */
    {0xD0, "BNE - Relative"},
    /* BMI */
    {0x30, "BMI - Relative"},
    /* BPL */
    {0x10, "BPL - Relative"},
    /* BVS */
    {0x70, "BVS - Relative"},
    /* BVC */
    {0x50, "BVC - Relative"},
    /* CMP */
    {0xC9, "CMP - Immediate"},
    {0xC5, "CMP - Zero page"},
    {0xD5, "CMP - Zero page X indexed"},
    {0xCD, "CMP - Absolute"},
    {0xDD, "CMP - Absolute X indexed"},
    {0xD9, "CMP - Absolute Y indexed"},
    {0xC1, "CMP - Indirect X indexed"},
    {0xD1, "CMP - Indirect Y indexed"},
    /* CPX */
    {0xE0, "CPX - Immediate"},
    {0xE4, "CPX - Zero page"},
    {0xEC, "CPX - Absolute"},
    /* CPY */
    {0xC0, "CPY - Immediate"},
    {0xC4, "CPY - Zero page"},
    {0xCC, "CPY - Absolute"},
    /* BIT */
    {0x24, "BIT - Zero page"},
    {0x2C, "BIT - Absolute"},
    /* ASL */
    {0x0A, "ASL - Accumulator"},
    {0x06, "ASL - Zero page"},
    {0x16, "ASL - Zero page X indexed"},
    {0x0E, "ASL - Absolute"},
    {0x1E, "ASL - Absolute X indexed"},
    /* LSR */
    {0x4A, "LSR - Accumulator"},
    {0x46, "LSR - Zero page"},
    {0x56, "LSR - Zero page X indexed"},
    {0x4E, "LSR - Absolute"},
    {0x5E, "LSR - Absolute X indexed"},
    /* ROL */
    {0x2A, "ROL - Accumulator"},
    {0x26, "ROL - Zero page"},
    {0x36, "ROL - Zero page X indexed"},
    {0x2E, "ROL - Absolute"},
    {0x3E, "ROL - Absolute X indexed"},
    /* ROR */
    {0x6A, "ROR - Accumulator"},
    {0x66, "ROR - Zero page"},
    {0x76, "ROR - Zero page X indexed"},
    {0x6E, "ROR - Absolute"},
    {0x7E, "ROR - Absolute X indexed"},
    /* TAX */
    {0xAA, "TAX - Implied"},
    /* TAY */
    {0xA8, "TAY - Implied"},
    /* TXA */
    {0x8A, "TXA - Implied"},
    /* TYA */
    {0x98, "TYA - Implied"},
    /* TSX */
    {0xBA, "TSX - Implied"},
    /* TXS */
    {0x9A, "TXS - Implied"},
    /* PHA */
    {0x48, "PHA - Implied"},
    /* PHP */
    {0x08, "PHP - Implied"},
    /* PLA */
    {0x68, "PLA - Implied"},
    /* PLP */
    {0x28, "PLP - Implied"},
    /* JSR */
    {0x20, "JSR - Implied"},
    /* RTS */
    {0x60, "RTS - Implied"},
    /* RTI */
    {0x40, "RTI - Implied"},
    /* CLC */
    {0x18, "CLC - Implied"},
    /* CLD */
    {0xD8, "CLD - Implied"},
    /* CLI */
    {0x58, "CLI - Implied"},
    /* CLV */
    {0xB8, "CLV - Implied"},
    /* SEC */
    {0x38, "SEC - Implied"},
    /* SED */
    {0xF8, "SED - Implied"},
    /* SEI */
    {0x78, "SEI - Implied"},
    /* NOP */
    {0xEA, "NOP - Implied"}
};

void debug_print_status_flags();
void debug_print_memory_contents(uint16_t address);
void debug_print_special_register(mos6507_register_t reg);
void debug_print_buses();
void debug_print_execution_step();
int debug_get_status_flag(uint8_t flag);
void debug_print_instruction();

#endif /* _DEBUG_H */
