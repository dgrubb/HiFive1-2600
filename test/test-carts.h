/*
 * File: test-carts.c
 * Author: dgrubb
 * Date: 07/28/2017
 *
 * Stores some very simple test programs for exercising parts of the 
 * CPU for test purposes.
 */

#ifndef _TEST_CARTS_H
#define _TEST_CARTS_H

#ifdef EXEC_TESTS

#include <stdint.h>

/******************************************************************************
 * LDA
 *****************************************************************************/

extern const uint8_t test_cart_LDA_Immediate[2];
extern const uint8_t test_cart_LDA_Zero_Page[2];
extern const uint8_t test_cart_LDA_Zero_Page_X_Indexed[2];
extern const uint8_t test_cart_LDA_Absolute[3];
extern const uint8_t test_cart_LDA_Absolute_X_Indexed[3];
extern const uint8_t test_cart_LDA_Absolute_X_Indexed_Boundary_Cross[3];
extern const uint8_t test_cart_LDA_Absolute_Y_Indexed[3];
extern const uint8_t test_cart_LDA_Absolute_Y_Indexed_Boundary_Cross[3];
extern const uint8_t test_cart_LDA_Indirect_X_Indexed[3];
extern const uint8_t test_cart_LDA_Indirect_Y_Indexed[3];

/******************************************************************************
 * LDX
 *****************************************************************************/

extern const uint8_t test_cart_LDX_Immediate[2];
extern const uint8_t test_cart_LDX_Zero_Page[2];
extern const uint8_t test_cart_LDX_Zero_Page_Y_Indexed[2];
extern const uint8_t test_cart_LDX_Absolute[3];
extern const uint8_t test_cart_LDX_Absolute_Y_Indexed[3];
extern const uint8_t test_cart_LDX_Absolute_Y_Indexed_Boundary_Cross[3];

/******************************************************************************
 * LDY
 *****************************************************************************/

extern const uint8_t test_cart_LDY_Immediate[2];
extern const uint8_t test_cart_LDY_Zero_Page[2];
extern const uint8_t test_cart_LDY_Zero_Page_X_Indexed[2];
extern const uint8_t test_cart_LDY_Absolute[3];
extern const uint8_t test_cart_LDY_Absolute_X_Indexed[3];
extern const uint8_t test_cart_LDY_Absolute_X_Indexed_Boundary_Cross[3];

/******************************************************************************
 * STA
 *****************************************************************************/

extern const uint8_t test_cart_STA_Zero_Page[4];
extern const uint8_t test_cart_STA_Zero_Page_X_Indexed[4];
extern const uint8_t test_cart_STA_Absolute[5];
extern const uint8_t test_cart_STA_Absolute_X_Indexed[5];
extern const uint8_t test_cart_STA_Absolute_Y_Indexed[5];
extern const uint8_t test_cart_STA_Indirect_X_Indexed[4];
extern const uint8_t test_cart_STA_Indirect_Y_Indexed[4];

/******************************************************************************
 * STX
 *****************************************************************************/

extern const uint8_t test_cart_STX_Zero_Page[4];
extern const uint8_t test_cart_STX_Zero_Page_Y_Indexed[4];
extern const uint8_t test_cart_STX_Absolute[5];

/******************************************************************************
 * STY
 *****************************************************************************/

extern const uint8_t test_cart_STY_Zero_Page[4];
extern const uint8_t test_cart_STY_Zero_Page_X_Indexed[4];
extern const uint8_t test_cart_STY_Absolute[5];

#endif /* EXEC_TESTS */

#endif /* _TEST_CARTS_H */
