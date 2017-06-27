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
typedef void (*fp)();

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
void opcode_execute(uint8_t opcode);

/* The following function prototypes define each possible opcodes from a
 * 6507 with nmemonic annotation in commens. An additional opcode ILL is
 * added to handle illegal codes placed into a program but has no analogue
 * in a real 6507 device.
 *
 * Annotation derived from explanations found at:
 * http://www.dwheeler.com/6502/oneelkruns/asm1step.html
 */

/* Load and store */
void opcode_LDA(); /* LoaD the Accumulator */
void opcode_LDX(); /* LoaD the X register */
void opcode_LDY(); /* LoaD the Y register */
void opcode_STA(); /* STore the Accumulator */
void opcode_STX(); /* STore the X register */
void opcode_STY(); /* STore the Y register */

/* Arithmetic */
void opcode_ADC(); /* ADd to Accumulator with Carry */
void opcode_SDC(); /* SuBtract from accumulator with Carry*/

/* Increment and decrement */
void opcode_INC(); /* INCrement memory by one */
void opcode_INX(); /* INcrement X by one */
void opcode_INY(); /* INcrement Y by one */
void opcode_DEC(); /* DECrement memory by one */
void opcode_DEX(); /* DEcrement X by one */
void opcode_DEX(); /* DEcrement X by one */
void opcode_DEY(); /* DEcrement Y by one */
void opcode_DEY(); /* DEcrement Y by one */

/* Logical */
void opcode_AND(); /* AND memowry with accumulator */
void opcode_ORA(addressing_mode_t address_mode); /* OR memory with Accumulator */
void opcode_EOR(); /* Exclusive-OR memory with accumulator */

/* Jump, branch, compare and test */
void opcode_JMP(); /* JuMP to another location (GOTO) */
void opcode_BCC(); /* Branch on Carry Clear */
void opcode_BCS(); /* Branch on Carry Set */
void opcode_BEQ(); /* Branch on EQual to zero */
void opcode_BNE(); /* Branch on Not Equal to zero */
void opcode_BMI(); /* Branch on MInus */
void opcode_BPL(addressing_mode_t address_mode); /* Branch on PLus */
void opcode_BVS(); /* Branch on oVerflow Set */
void opcode_BVC(); /* Branch on oVerflow clear */
void opcode_CMP(); /* CoMPare memory and accumulator */
void opcode_CPX(); /* ComPare memory and X */
void opcode_CPY(); /* ComPare memory and Y*/
void opcode_BIT(); /* Test BITs */

/* Shift and rotate */
void opcode_ASL(addressing_mode_t address_mode); /* Accumulator Shift Left */
void opcode_LSR(); /* Logical Shift Right */
void opcode_ROL(); /* ROtate Left */
void opcode_ROR(); /* ROtate Right */

/* Transfer */
void opcode_TAX(); /* Transfer Accumulator to X */
void opcode_TAY(); /* Transfer Accumulator to Y */
void opcode_TXA(); /* Transfer X to Accumulator */
void opcode_TYA(); /* Transfer Y to Accumulator */

/* Stack */
void opcode_TSX(); /* Transfer Stack pointer to X */
void opcode_TXS(); /* Transfer X to Stack pointer */
void opcode_PHA(); /* PusH Accumulator on stack */
void opcode_PHP(addressing_mode_t address_mode); /* PusH Processor status on stack */
void opcode_PLA(); /* PulL Accumulator from stack */
void opcode_PLP(); /* PulL Processor status from stack */

/* Subroutine */
void opcode_JSR(); /* Jump to SubRoutine */
void opcode_RTS(); /* ReTurn from Subroutine */
void opcode_RTI(); /* ReTurn from Interrupt */

/* Set and reset */
void opcode_CLC(addressing_mode_t address_mode); /* CLear Carry flag */
void opcode_CLD(); /* CLear Decimal mode */
void opcode_CLI(); /* CLear Interrupt disable */
void opcode_CLV(); /* CLear oVerflow flag */

void opcode_SEC(); /* SEt Carry */
void opcode_SED(); /* SEt Decimal mode */
void opcode_SEI(); /* SEt Interrupt disable */

/* Miscellaneous */
void opcode_NOP(); /* No OPeration */
void opcode_BRK(addressing_mode_t address_mode); /* BReaK */

/* This is part of the program logic rather than the 6507 model. It
 * provides a sink when illegal opcodes are invoked. Further work
 * could include fleshing out the illegal opcodes with their real-world
 * actions depending on which specific 6507 implementation is being
 * emulated?
 */
void opcode_ILL(addressing_mode_t address_mode); /* Illegal */

#endif /* _MOS6507_OPCODES_H */
