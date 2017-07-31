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
    uint8_t data = 0;

    /* Perform test */
    cartridge_load(test_cart_LDA_Immediate);
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the immediate operand */
    /* End test */

    /* Do we have the expected result (0xAA) in the Accumulator? */
    mos6507_get_register(MOS6507_REG_A, &data);
    assert(data == 0xAA);
}
void test_LDA_Zero_Page()
{
    RESET()
    uint8_t data = 0;

    /* Setup the test by pre-loaing our test data into memory */
    mos6507_set_data_bus(0xAA);
    mos6507_set_address_bus(0x0081);

    /* Now load our test program and start clocking the CPU */
    cartridge_load(test_cart_LDA_Zero_Page);
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Fetch the value from memory and load it */
    /* End test */

    /* Do we have the expected result (0xAA) in the Accumulator? */
    mos6507_get_register(MOS6507_REG_A, &data);
    assert(data == 0xAA);
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
    uint8_t data = 0;

    /* Perform test */
    cartridge_load(test_cart_LDX_Immediate);
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the immediate operand */
    /* End test */

    /* Do we have the expected result (0xAA) in the X register? */
    mos6507_get_register(MOS6507_REG_X, &data);
    assert(data == 0xAA);
}

void test_LDX_Zero_Page()
{
    RESET()
    uint8_t data = 0;

    /* Perform test */
    cartridge_load(test_cart_LDX_Zero_Page);
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Fetch the value from memory and load it */
    /* End test */

    /* Do we have the expected result (0xAA) in the X register? */
    mos6507_get_register(MOS6507_REG_X, &data);
    assert(data == 0xAA);
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
    uint8_t data = 0;

    /* Perform test */
    cartridge_load(test_cart_LDY_Immediate);
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the immediate operand */
    /* End test */

    /* Do we have the expected result (0xAA) in the Y register? */
    mos6507_get_register(MOS6507_REG_Y, &data);
    assert(data == 0xAA);

}

#endif /* EXEC_TESTS */
