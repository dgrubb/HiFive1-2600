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
#include "../atari/Atari-memmap.h"
#include "../cpu/mos6507.h"
#include "../memory/mos6532.h"
#include "debug.h"
#include "test-carts.h"

#define RESET() \
    mos6532_clear_memory(); \
    mos6507_reset(); \

void execute_tests()
{
    puts("============ Executing unit tests ============\n\r");

    test_LDA();

    puts("====== All tests completed successfully ======\n\r");
}

void insert_test_data(uint16_t address, uint8_t data)
{
    uint16_t current_address;
    uint8_t current_data;

    /* Save the current adddress and data bus states. Switch both to the 
     * specified data and address, write to memory and then set it back to
     * the previous state. This all happens without the use of invoking a 
     * CPU clock tick so appears seamlessly.
     */
    mos6507_get_address_bus(&current_address);
    mos6507_get_data_bus(&current_data);
    mos6507_set_data_bus(data);
    mos6507_set_address_bus(address);
    memmap_write();
    mos6507_set_data_bus(current_data);
    mos6507_set_address_bus(current_address);
}

/******************************************************************************
 * Load the accumulator
 *****************************************************************************/

void test_LDA()
{
    puts("--- Testing LDA:");

    test_LDA_Immediate();
    test_LDA_Zero_Page();
    test_LDA_Zero_Page_X_Indexed();
    test_LDA_Absolute();
    test_LDA_Absolute_X_Indexed();
    test_LDA_Absolute_X_Indexed_Boundary_Cross();
    test_LDA_Absolute_Y_Indexed();
    test_LDA_Absolute_Y_Indexed_Boundary_Cross();
    test_LDA_Indirect_X_Indexed();

    puts("--- All LDA tests completed successfully.\n\r");
}

void test_LDA_Immediate()
{
    puts("+ Testing LDA, immediate addressing mode: 0xA9");
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
    puts("+ Testing LDA, zero page addressing mode: 0xA5");
    RESET()
    uint8_t data = 0;
    uint16_t current_address;
    uint8_t current_data;

    /* Setup the test by pre-loading our test data into memory */
    insert_test_data(0x0081, 0xAA);

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

void test_LDA_Zero_Page_X_Indexed()
{
    puts("+ Testing LDA, zero page X indexed addressing mode: 0xB5");
    RESET()
    uint8_t data = 0;
    uint16_t current_address;
    uint8_t current_data;

    /* Setup the test by pre-loading our test data into memory */
    insert_test_data(0x0091, 0xAA);

    /* The cartridge specifies 0x90, this offset gets us to the correct location */
    mos6507_set_register(MOS6507_REG_X, 0x01);

    /* Now load our test program and start clocking the CPU */
    cartridge_load(test_cart_LDA_Zero_Page_X_Indexed);
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Fetch the value from memory and use it as the next address */
    mos6507_clock_tick(); /* Fetch the X register and index it with the address */
    /* End test */

    /* Do we have the expected result (0xAA) in the Accumulator? */
    mos6507_get_register(MOS6507_REG_A, &data);
    assert(data == 0xAA);
}

void test_LDA_Absolute()
{
    puts("+ Testing LDA, absolute addressing mode: 0xAD");
    RESET()
    uint8_t data = 0;

    /* Setup the test by pre-loading our test data into memory */
    insert_test_data(0x2001, 0xAA);

    /* Now load our test program and start clocking the CPU */
    cartridge_load(test_cart_LDA_Absolute);
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the low memory location */
    mos6507_clock_tick(); /* Fetch the next byte for the high memory location */
    mos6507_clock_tick(); /* Fetch the value from memory and load it */
    /* End test */

    /* Do we have the expected result (0xAA) in the Accumulator? */
    mos6507_get_register(MOS6507_REG_A, &data);
    assert(data == 0xAA);
}

void test_LDA_Absolute_X_Indexed()
{
    puts("+ Testing LDA, absolute X indexed addressing mode: 0xAD");
    RESET()
    uint8_t data = 0;

    /* Setup the test by pre-loading our test data into memory */
    insert_test_data(0x2001, 0xAA);

    /* The cartridge specifies 0x2000, this offset gets us to the correct location */
    mos6507_set_register(MOS6507_REG_X, 0x01);

    /* Now load our test program and start clocking the CPU */
    cartridge_load(test_cart_LDA_Absolute_X_Indexed);
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the low memory location */
    mos6507_clock_tick(); /* Fetch the next byte for the high memory location */
    mos6507_clock_tick(); /* Fetch the value from memory and load it */
    /* End test */

    /* Do we have the expected result (0xAA) in the Accumulator? */
    mos6507_get_register(MOS6507_REG_A, &data);
    assert(data == 0xAA);
}

void test_LDA_Absolute_X_Indexed_Boundary_Cross()
{
    puts("+ Testing LDA, absolute X indexed addressing mode with boundary crossing: 0xAD");
    RESET()
    uint8_t data = 0;

    /* Setup the test by pre-loading our test data into memory */
    insert_test_data(0x0100, 0xAA);

    /* The cartridge specifies 0x2000, this offset gets us to the correct location */
    mos6507_set_register(MOS6507_REG_X, 0x01);

    /* Now load our test program and start clocking the CPU */
    cartridge_load(test_cart_LDA_Absolute_X_Indexed_Boundary_Cross);
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the low memory location */
    mos6507_clock_tick(); /* Fetch the next byte for the high memory location */
    mos6507_clock_tick(); /* Compute new address with carry */
    mos6507_clock_tick(); /* Fetch the value from memory and load it */
    /* End test */

    /* Do we have the expected result (0xAA) in the Accumulator? */
    mos6507_get_register(MOS6507_REG_A, &data);
    assert(data == 0xAA);
}

void test_LDA_Absolute_Y_Indexed()
{
    puts("+ Testing LDA, absolute Y indexed addressing mode: 0xA9");
    RESET()
    uint8_t data = 0;

    /* Setup the test by pre-loading our test data into memory */
    insert_test_data(0x2001, 0xAA);

    /* The cartridge specifies 0x2000, this offset gets us to the correct location */
    mos6507_set_register(MOS6507_REG_Y, 0x01);

    /* Now load our test program and start clocking the CPU */
    cartridge_load(test_cart_LDA_Absolute_Y_Indexed);
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the low memory location */
    mos6507_clock_tick(); /* Fetch the next byte for the high memory location */
    mos6507_clock_tick(); /* Fetch the value from memory and load it */
    /* End test */

    /* Do we have the expected result (0xAA) in the Accumulator? */
    mos6507_get_register(MOS6507_REG_A, &data);
    assert(data == 0xAA);
}

void test_LDA_Absolute_Y_Indexed_Boundary_Cross()
{
    puts("+ Testing LDA, absolute Y indexed addressing mode with boundary crossing: 0xA9");
    RESET()
    uint8_t data = 0;

    /* Setup the test by pre-loading our test data into memory */
    insert_test_data(0x0100, 0xAA);

    /* The cartridge specifies 0x2000, this offset gets us to the correct location */
    mos6507_set_register(MOS6507_REG_Y, 0x01);

    /* Now load our test program and start clocking the CPU */
    cartridge_load(test_cart_LDA_Absolute_Y_Indexed_Boundary_Cross);
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the low memory location */
    mos6507_clock_tick(); /* Fetch the next byte for the high memory location */
    mos6507_clock_tick(); /* Compute new address with carry */
    mos6507_clock_tick(); /* Fetch the value from memory and load it */
    /* End test */

    /* Do we have the expected result (0xAA) in the Accumulator? */
    mos6507_get_register(MOS6507_REG_A, &data);
    assert(data == 0xAA);
}

void test_LDA_Indirect_X_Indexed()
{
    puts("+ Testing LDA, indirect X indexed addressing mode: 0xA1");
    RESET()
    uint8_t data = 0;

    /* Setup the test by pre-loading our test data into memory */
    insert_test_data(0x0090, 0x91);
    insert_test_data(0x0091, 0x00);
    insert_test_data(0x0092, 0xAA);

    mos6507_set_register(MOS6507_REG_X, 0x01);

    /* Now load our test program and start clocking the CPU */
    cartridge_load(test_cart_LDA_Indirect_X_Indexed);
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the zero page base address */
    mos6507_clock_tick(); /* Set the data bus */
    mos6507_clock_tick(); /* Apply X offset and fetch low byte */
    mos6507_clock_tick(); /* Fetch high byte */
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
