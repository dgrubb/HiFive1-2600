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
    0xA6, /* LDX, Load accumulator with ... */
    0x81  /* ... the contents of this zero-page RAM location */
};

uint8_t test_cart_LDX_Zero_Page_Y_Indexed[] = {
    0xB6, /* LDX, Load accumulator with ... */
    0x90  /* ... the contents of this zero-page RAM location, +Y index register */
};

uint8_t test_cart_LDX_Absolute[] = {
    0xAE, /* LDX, Load accumulator with ... */
    0x01, /* ... the contents of this low address byte ... */
    0x20  /* ... and this high address byte ... */
};

uint8_t test_cart_LDX_Absolute_Y_Indexed[] = {
    0xAE, /* LDX, Load X index with ... */
    0x00, /* ... the contents of this low address byte ... */
    0x20  /* ... and this high address byte ... */
};

uint8_t test_cart_LDX_Absolute_Y_Indexed_Boundary_Cross[] = {
    0xBE, /* LDA, Load X index with ... */
    0xFF, /* ... the contents of this low address byte ... */
    0x00  /* ... and this high address byte ... */
};

#endif /* EXEC_TESTS */

#endif /* _TEST_CARTS_H */
