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

uint8_t test_cart_LDX_Immediate[] = {
    0xA2, /* LDX, Load accumulator with ... */
    0xAA  /* ... the raw value 0xAA */
};

uint8_t test_cart_LDX_Zero_Page[] = {
    0xA9, /* LDX, Load accumulator with ... */
    0x81  /* ... the contents of this zero-page RAM location */
};

uint8_t test_cart_LDY_Immediate[] = {
    0xA0, /* LDY, Load accumulator with ... */
    0xAA  /* ... the raw value 0xAA */
};

#endif /* EXEC_TESTS */

#endif /* _TEST_CARTS_H */
