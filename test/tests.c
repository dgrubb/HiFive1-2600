/*
 * File: tests.c
 * Author: dgrubb
 * Date: 07/17/2017
 *
 * Adds a sequence of tests to verify emulation is correct.
 */

#include "tests.h"

#ifdef EXEC_TESTS

#include <assert.h>
#include <stdio.h>
#include "../atari/Atari-cart.h"
#include "../cpu/mos6507.h"
#include "../memory/mos6532.h"
#include "test-carts.h"

#define RESET() \
    mos6532_clear_memory(); \
    mos6507_reset(); \

void execute_tests()
{
    test_LDA();

    puts("All tests completed successfully.");
}

/******************************************************************************
 * Load the accumulator
 *****************************************************************************/

void test_LDA()
{
    test_LDA_Immediate();
    test_LDA_Zero_Page();

    puts("All LDA tests completed successfully.");
}

void test_LDA_Immediate()
{
    RESET()
    cartridge_load(test_cart_LDA_Immediate);
}

void test_LDA_Zero_Page()
{
}

/******************************************************************************
 * Load the X register
 *****************************************************************************/

void test_LDX()
{
    test_LDX_Immediate();

    puts("All LDX tests completed successfully.");
}

void test_LDX_Immediate()
{
    RESET()
}

/******************************************************************************
 * Load the Y register
 *****************************************************************************/

void test_LDY()
{
    test_LDY_Immediate();

    puts("All LDY tests completed successfully.");
}

void test_LDY_Immediate()
{
    RESET()
}

#endif /* EXEC_TESTS */
