/*
 * File: tests.c
 * Author: dgrubb
 * Date: 07/17/2017
 *
 * Adds a sequence of tests to verify emulation is correct.
 */

#include "tests.h"

#ifdef EXEC_TESTS

#include <stdio.h>
#include "mos6507.h"
#include "mos6532.h"

#define RESET() \
    mos6532_clear_memory(); \
    mos6507_reset(); \

int execute_tests()
{
    if (test_LDA()) goto err;

    puts("All tests completed successfully.");
    return 0;

err:
    puts("Error while executing test cases.");
    return -1;
}

/******************************************************************************
 * Load the accumulator
 *****************************************************************************/

int test_LDA()
{
    if (test_LDA_Immediate()) goto lda_err;
    if (test_LDA_Zero_Page()) goto lda_err;

    puts("All LDA tests completed successfully.");
    return 0;

lda_err:
    puts("Error in LDA test result.");
    return -1;
}

int test_LDA_Immediate()
{
    
    return 0;
}

int test_LDA_Zero_Page()
{
    return 0;
}

/******************************************************************************
 * Load the X register
 *****************************************************************************/

int test_LDX()
{
    if (test_LDX_Immediate()) goto ldx_err;

    puts("All LDX tests completed successfully.");
    return 0;

ldx_err:
    puts("Error in LDX test result.");
    return -1;
}

/******************************************************************************
 * Load the Y register
 *****************************************************************************/

int test_LDY()
{
    if (test_LDY_Immediate()) goto ldy_err;

    puts("All LDY tests completed successfully.");
    return 0;

ldy_err:
    puts("Error in LDY test result.");
    return -1;
}

#endif /* EXEC_TESTS */
