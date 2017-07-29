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

uint8_t test_cart_LDA_Immediate = {
    0xA9, /* LDA, Load accumulator with ... */
    0xAA  /* ... the raw value 0xAA */
};

#endif /* EXEC_TESTS */

#endif /* _TEST_CARTS_H */
