/*
 * File: mos6507-opcodes.h
 * Author: dgrubb
 * Date: 12/20/2016
 *
 * Provides implementations of the 6507's op-codes.
 */

#ifndef _MOS6507_OPCODES_H
#define _MOS6507_OPCODES_H

/* Opcodes are 8-bit, allowing for 255 unique permutations.
 * However, many bit selections don't represent valid operations.
 */
#define ISA_LENGTH 255

/* Define a function pointer type */
typedef int (*fp)();

typedef enum {
    OPCODE_ADDRESSING_MODE_ACCUMULATOR = 0,
    OPCODE_ADDRESSING_MODE_ABSOLUTE,
    OPCODE_ADDRESSING_MODE_ABSOLUTE_X_INDEXED,
    OPCODE_ADDRESSING_MODE_ABSOLUTE_Y_INDEXED,
    OPCODE_ADDRESSING_MODE_IMMEDIATE,
    OPCODE_ADDRESSING_MODE_IMPLIED,
    OPCODE_ADDRESSING_MODE_INDIRECT,
    OPCODE_ADDRESSING_MODE_X_INDEXED_INDIRECT,
    OPCODE_ADDRESSING_MODE_INDIRECT_Y_INDEXED,
    OPCODE_ADDRESSING_MODE_RELATIVE,
    OPCODE_ADDRESSING_MODE_ZERO_PAGE,
    OPCODE_ADDRESSING_MODE_ZERO_PAGE_X_INDEXED,
    OPCODE_ADDRESSING_MODE_ZERO_PAGE_Y_INDEXED,
} addressing_mode_t;

typedef struct {
    fp opcode;
    addressing_mode_t addressing_mode;
} instruction_t;

extern instruction_t ISA_table[ISA_LENGTH];

void opcode_populate_ISA_table();
int opcode_execute(uint8_t opcode);

/* The following function prototypes define each possible opcodes from a
 * 6507 with nmemonic annotation in commens. An additional opcode ILL is
 * added to handle illegal codes placed into a program but has no analogue
 * in a real 6507 device.
 *
 * Annotation derived from explanations found at:
 * http://www.dwheeler.com/6502/oneelkruns/asm1step.html
 */

/* Load and store */
int opcode_LDA(); /* LoaD the Accumulator */
int opcode_LDX(); /* LoaD the X register */
int opcode_LDY(); /* LoaD the Y register */
int opcode_STA(); /* STore the Accumulator */
int opcode_STX(); /* STore the X register */
int opcode_STY(); /* STore the Y register */

/* Arithmetic */
int opcode_ADC(); /* ADd to Accumulator with Carry */
int opcode_SDC(); /* SuBtract from accumulator with Carry*/

/* Increment and decrement */
int opcode_INC(); /* INCrement memory by one */
int opcode_INX(); /* INcrement X by one */
int opcode_INY(); /* INcrement Y by one */
int opcode_DEC(); /* DECrement memory by one */
int opcode_DEX(); /* DEcrement X by one */
int opcode_DEX(); /* DEcrement X by one */
int opcode_DEY(); /* DEcrement Y by one */
int opcode_DEY(); /* DEcrement Y by one */

/* Logical */
int opcode_AND(); /* AND memowry with accumulator */
int opcode_ORA(int cycle, addressing_mode_t address_mode); /* OR memory with Accumulator */
int opcode_EOR(); /* Exclusive-OR memory with accumulator */

/* Jump, branch, compare and test */
int opcode_JMP(); /* JuMP to another location (GOTO) */
int opcode_BCC(); /* Branch on Carry Clear */
int opcode_BCS(); /* Branch on Carry Set */
int opcode_BEQ(); /* Branch on EQual to zero */
int opcode_BNE(); /* Branch on Not Equal to zero */
int opcode_BMI(); /* Branch on MInus */
int opcode_BPL(int cycle, addressing_mode_t address_mode); /* Branch on PLus */
int opcode_BVS(); /* Branch on oVerflow Set */
int opcode_BVC(); /* Branch on oVerflow clear */
int opcode_CMP(); /* CoMPare memory and accumulator */
int opcode_CPX(); /* ComPare memory and X */
int opcode_CPY(); /* ComPare memory and Y*/
int opcode_BIT(); /* Test BITs */

/* Shift and rotate */
int opcode_ASL(int cycle, addressing_mode_t address_mode); /* Accumulator Shift Left */
int opcode_LSR(); /* Logical Shift Right */
int opcode_ROL(); /* ROtate Left */
int opcode_ROR(); /* ROtate Right */

/* Transfer */
int opcode_TAX(); /* Transfer Accumulator to X */
int opcode_TAY(); /* Transfer Accumulator to Y */
int opcode_TXA(); /* Transfer X to Accumulator */
int opcode_TYA(); /* Transfer Y to Accumulator */

/* Stack */
int opcode_TSX(); /* Transfer Stack pointer to X */
int opcode_TXS(); /* Transfer X to Stack pointer */
int opcode_PHA(); /* PusH Accumulator on stack */
int opcode_PHP(int cycle, addressing_mode_t address_mode); /* PusH Processor status on stack */
int opcode_PLA(); /* PulL Accumulator from stack */
int opcode_PLP(); /* PulL Processor status from stack */

/* Subroutine */
int opcode_JSR(); /* Jump to SubRoutine */
int opcode_RTS(); /* ReTurn from Subroutine */
int opcode_RTI(); /* ReTurn from Interrupt */

/* Set and reset */
int opcode_CLC(int cycle, addressing_mode_t address_mode); /* CLear Carry flag */
int opcode_CLD(); /* CLear Decimal mode */
int opcode_CLI(); /* CLear Interrupt disable */
int opcode_CLV(); /* CLear oVerflow flag */

int opcode_SEC(); /* SEt Carry */
int opcode_SED(); /* SEt Decimal mode */
int opcode_SEI(); /* SEt Interrupt disable */

/* Miscellaneous */
int opcode_NOP(); /* No OPeration */
int opcode_BRK(int cycle, addressing_mode_t address_mode); /* BReaK */

/* This is part of the program logic rather than the 6507 model. It
 * provides a sink when illegal opcodes are invoked. Further work
 * could include fleshing out the illegal opcodes with their real-world
 * actions depending on which specific 6507 implementation is being
 * emulated?
 */
int opcode_ILL(int cycle, addressing_mode_t address_mode); /* Illegal */

#endif /* _MOS6507_OPCODES_H */
