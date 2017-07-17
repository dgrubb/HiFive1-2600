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
#include "mos6507.h"
#include "mos6532.h"

#define RESET() \
    mos6532_clear_memory(); \
    mos6507_reset(); \

void execute_tests()
{
    test_LDA();
}

void test_LDA()
{
    RESET();
    if (-1 == test_LDA_Immediate()) {
        puts("Error");
    } else {
        puts("Success!");
    }
}

int test_LDA_Immediate()
{
    assert(1);
    return 0;
}

#endif /* EXEC_TESTS */
