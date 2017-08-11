/*
 * File: test-carts.h
 * Author: dgrubb
 * Date: 07/28/2017
 *
 * Stores some very simple test programs for exercising parts of the 
 * CPU for test purposes.
 */


#ifndef _TEST_CARTS_H
#define _TEST_CARTS_H

#ifdef EXEC_TESTS

/******************************************************************************
 * LDA
 *****************************************************************************/

uint8_t test_cart_LDA_Immediate[] = {
    0xA9, /* LDA, Load accumulator with ... */
    0xAA  /* ... the raw value 0xAA */
};

uint8_t test_cart_LDA_Zero_Page[] = {
    0xA5, /* LDA, Load accumulator with ... */
    0x81  /* ... the contents of this zero-page RAM location */
};

uint8_t test_cart_LDA_Zero_Page_X_Indexed[] = {
    0xB5, /* LDA, Load accumulator with ... */
    0x90  /* ... the contents of this zero-page RAM location, +X index register */
};

uint8_t test_cart_LDA_Absolute[] = {
    0xB5, /* LDA, Load accumulator with ... */
    0x01, /* ... the contents of this low address byte ... */
    0x20  /* ... and this high address byte ... */
};

uint8_t test_cart_LDA_Absolute_X_Indexed[] = {
    0xBD, /* LDA, Load accumulator with ... */
    0x00, /* ... the contents of this low address byte ... */
    0x20  /* ... and this high address byte ... */
};

uint8_t test_cart_LDA_Absolute_X_Indexed_Boundary_Cross[] = {
    0xBD, /* LDA, Load accumulator with ... */
    0xFF, /* ... the contents of this low address byte ... */
    0x00  /* ... and this high address byte ... */
};

uint8_t test_cart_LDA_Absolute_Y_Indexed[] = {
    0xB9, /* LDA, Load accumulator with ... */
    0x00, /* ... the contents of this low address byte ... */
    0x20  /* ... and this high address byte ... */
};

uint8_t test_cart_LDA_Absolute_Y_Indexed_Boundary_Cross[] = {
    0xB9, /* LDA, Load accumulator with ... */
    0xFF, /* ... the contents of this low address byte ... */
    0x00  /* ... and this high address byte ... */
};

uint8_t test_cart_LDA_Indirect_X_Indexed[] = {
    0xA1, /* LDA, Load accumulator with ... */
    0x90, /* ... an address in zero-page which ... */
    0x00  /* ... combined with the X index register yeilds the actual data */
};

uint8_t test_cart_LDA_Indirect_Y_Indexed[] = {
    0xA1, /* LDA, Load accumulator with ... */
    0x90, /* ... an address in zero-page which ... */
    0x00  /* ... combined with the Y index register yeilds the actual data */
};

/******************************************************************************
 * LDX
 *****************************************************************************/

uint8_t test_cart_LDX_Immediate[] = {
    0xA2, /* LDX, Load X index with ... */
    0xAA  /* ... the raw value 0xAA */
};

uint8_t test_cart_LDX_Zero_Page[] = {
    0xA6, /* LDX, Load X index with ... */
    0x81  /* ... the contents of this zero-page RAM location */
};

uint8_t test_cart_LDX_Zero_Page_Y_Indexed[] = {
    0xB6, /* LDX, Load X index with ... */
    0x90  /* ... the contents of this zero-page RAM location, +Y index register */
};

uint8_t test_cart_LDX_Absolute[] = {
    0xAE, /* LDX, Load X index with ... */
    0x01, /* ... the contents of this low address byte ... */
    0x20  /* ... and this high address byte ... */
};

uint8_t test_cart_LDX_Absolute_Y_Indexed[] = {
    0xAE, /* LDX, Load X index with ... */
    0x00, /* ... the contents of this low address byte ... */
    0x20  /* ... and this high address byte ... */
};

uint8_t test_cart_LDX_Absolute_Y_Indexed_Boundary_Cross[] = {
    0xBE, /* LDX, Load X index with ... */
    0xFF, /* ... the contents of this low address byte ... */
    0x00  /* ... and this high address byte ... */
};

/******************************************************************************
 * LDY
 *****************************************************************************/

uint8_t test_cart_LDY_Immediate[] = {
    0xA0, /* LDY, Load Y index with ... */
    0xAA  /* ... the raw value 0xAA */
};

uint8_t test_cart_LDY_Zero_Page[] = {
    0xA4, /* LDY, Load Y index with ... */
    0x81  /* ... the contents of this zero-page RAM location */
};

uint8_t test_cart_LDY_Zero_Page_X_Indexed[] = {
    0xB4, /* LDY, Load Y index with ... */
    0x90  /* ... the contents of this zero-page RAM location, +Y index register */
};

uint8_t test_cart_LDY_Absolute[] = {
    0xAC, /* LDY, Load Y index with ... */
    0x01, /* ... the contents of this low address byte ... */
    0x20  /* ... and this high address byte ... */
};

uint8_t test_cart_LDY_Absolute_X_Indexed[] = {
    0xBC, /* LDY, Load Y index with ... */
    0x00, /* ... the contents of this low address byte ... */
    0x20  /* ... and this high address byte ... */
};

uint8_t test_cart_LDY_Absolute_X_Indexed_Boundary_Cross[] = {
    0xBC, /* LDY, Load Y index with ... */
    0xFF, /* ... the contents of this low address byte ... */
    0x00  /* ... and this high address byte ... */
};

/******************************************************************************
 * STA
 *****************************************************************************/

uint8_t test_cart_STA_Zero_Page[] = {
    0xA9, /* LDA, load accumulator with ... */
    0xBB, /* ... this value */
    0x85, /* ... then move that value */
    0x99  /* ... into this location in memory */
};

uint8_t test_cart_STA_Zero_Page_X_Indexed[] = {
    0xA9, /* LDA, load accumulator with ... */
    0xBB, /* ... this value */
    0x95, /* Then move the accumulator contents ... */
    0x90 /* ... to the offset of this address + index register*/
};

uint8_t test_cart_STA_Absolute[] = {
    0xA9, /* LDA, load accumulator with ... */
    0xBB, /* ... this value */
    0x8D, /* Then move the accumulator contents ... */
    0xFF, /* ... to this ...*/
    0x01  /* ... address */
};

uint8_t test_cart_STA_Absolute_X_Indexed[] = {
    0xA9, /* LDA, load accumulator with ... */
    0xBB, /* ... this value */
    0x9D, /* The move the accumulator contents ... */
    0xF0, /* ... low byte ... */
    0x01 /* ... high byte ... */
};

uint8_t test_cart_STA_Absolute_Y_Indexed[] = {
    0x99, /* LDA, load accumulator with ... */
    0xBB, /* ... this value */
    0x9D, /* The move the accumulator contents ... */
    0xF0, /* ... low byte ... */
    0x01 /* ... high byte ... */
};

uint8_t test_cart_STA_Indirect_X_Indexed[] = {
    0x99, /* LDA, load accumulator with ... */
    0xBB, /* ... this value */
    0x81, /* Then move accumulator contents ... */
    0x90  /* ... to this address + index offset*/
};

uint8_t test_cart_STA_Indirect_Y_Indexed[] = {
    0x99, /* LDA, load accumulator with ... */
    0xBB, /* ... this value */
    0x91, /* Then move accumulator contents ... */
    0x90  /* ... to this address + index offset*/
};

/******************************************************************************
 * STX
 *****************************************************************************/

uint8_t test_cart_STX_Zero_Page[] = {
    0xA2, /* LDX, load X index with ... */
    0xBB, /* ... this value */
    0x86, /* Then read that value ... */
    0x99, /* .. into this zero page location */
};

uint8_t test_cart_STX_Zero_Page_Y_Indexed[] = {
    0xA2, /* LDX, load X index with ... */
    0xBB, /* ... this value */
    0x96, /* Then read that value ... */
    0x90, /* .. into this zero page location */
};

uint8_t test_cart_STX_Absolute[] = {
    0xA2, /* LDX, load X index with ... */
    0xBB, /* ... this value */
    0x8E, /* Then read that value ... */
    0xFF, /* .. into this low byte ... */
    0x01  /* ... and this high byte */
};

/******************************************************************************
 * STY
 *****************************************************************************/

uint8_t test_cart_STY_Zero_Page[] = {
    0xA0, /* LDY, load Y index with ... */
    0xBB, /* ... this value */
    0x84, /* Then read that value ... */
    0x99, /* .. into this zero page location */
};

uint8_t test_cart_STY_Zero_Page_X_Indexed[] = {
    0xA0, /* LDY, load Y index with ... */
    0xBB, /* ... this value */
    0x94, /* Then read that value ... */
    0x90, /* .. into this zero page location */
};

uint8_t test_cart_STY_Absolute[] = {
    0xA0, /* LDY, load Y index with ... */
    0xBB, /* ... this value */
    0x8C, /* Then read that value ... */
    0xFF, /* .. into this low byte ... */
    0x01  /* ... and this high byte */
};

#endif /* EXEC_TESTS */

#endif /* _TEST_CARTS_H */
