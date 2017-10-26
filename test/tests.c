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
#include "atari/Atari-cart.h"
#include "atari/Atari-memmap.h"
#include "cpu/mos6507.h"
#include "memory/mos6532.h"
#include "debug.h"
#include "test-carts.h"

#define RESET() \
    mos6532_clear_memory(); \
    mos6507_reset(); \

void execute_tests(void)
{
    puts("============ Executing unit tests ============");

    test_LDA();
    test_LDX();
    test_LDY();
    test_STA();
    test_STX();
    test_STY();

    puts("====== All tests completed successfully ======");
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

void read_test_data(uint16_t address, uint8_t *data)
{
    uint16_t current_address;
    uint8_t current_data;

    /* Save the current address and data bus states. Switch to the specified
     * address and read out of memory. Once complete return the address and 
     * data bus to their previous states.
     */
    mos6507_get_address_bus(&current_address);
    mos6507_get_data_bus(&current_data);
    mos6507_set_address_bus(address);
    memmap_read(data);
    mos6507_set_data_bus(current_data);
    mos6507_set_address_bus(current_address);
}

/******************************************************************************
 * Load the accumulator
 *****************************************************************************/

void test_LDA(void)
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
    test_LDA_Indirect_Y_Indexed();

    puts("--- All LDA tests completed successfully.");
}

void test_LDA_Immediate(void)
{
    puts("+ Testing LDA [ 0xA9 ], immediate addressing mode");
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
void test_LDA_Zero_Page(void)
{
    puts("+ Testing LDA [ 0xA5 ], zero page addressing mode");
    RESET()
    uint8_t data = 0;

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

void test_LDA_Zero_Page_X_Indexed(void)
{
    puts("+ Testing LDA [ 0xB5 ], zero page X indexed addressing mode");
    RESET()
    uint8_t data = 0;

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

void test_LDA_Absolute(void)
{
    puts("+ Testing LDA [ 0xAD ], absolute addressing mode");
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

void test_LDA_Absolute_X_Indexed(void)
{
    puts("+ Testing LDA [ 0xBD ], absolute X indexed addressing mode");
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

void test_LDA_Absolute_X_Indexed_Boundary_Cross(void)
{
    puts("+ Testing LDA [ 0xAD ], absolute X indexed addressing mode with boundary crossing");
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

void test_LDA_Absolute_Y_Indexed(void)
{
    puts("+ Testing LDA [ 0xA9 ], absolute Y indexed addressing mode");
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

void test_LDA_Absolute_Y_Indexed_Boundary_Cross(void)
{
    puts("+ Testing LDA [ 0xA9 ], absolute Y indexed addressing mode with boundary crossing");
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

void test_LDA_Indirect_X_Indexed(void)
{
    puts("+ Testing LDA [ 0xA1 ], indirect X indexed addressing mode");
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

void test_LDA_Indirect_Y_Indexed(void)
{
    puts("+ Testing LDA [ 0xB1 ], indirect Y indexed addressing mode");
    RESET()
    uint8_t data = 0;

    /* Setup the test by pre-loading our test data into memory */
    insert_test_data(0x0090, 0x91);
    insert_test_data(0x0091, 0x95);
    insert_test_data(0x0092, 0x00);
    insert_test_data(0x0097, 0xAA);

    mos6507_set_register(MOS6507_REG_Y, 0x02);

    /* Now load our test program and start clocking the CPU */
    cartridge_load(test_cart_LDA_Indirect_Y_Indexed);
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the address offset */
    mos6507_clock_tick(); /* Apply the offset and read the low byte */
    mos6507_clock_tick(); /* Increment address and read high byte */
    mos6507_clock_tick(); /* Apply Y index offset */
    mos6507_clock_tick(); /* Fetch data */
    /* End test */

    /* Do we have the expected result (0xAA) in the Accumulator? */
    mos6507_get_register(MOS6507_REG_A, &data);
    assert(data == 0xAA);
}

/******************************************************************************
 * Load the X register
 *****************************************************************************/

void test_LDX(void)
{
    puts("--- Testing LDX:");

    test_LDX_Immediate();
    test_LDX_Zero_Page();
    test_LDX_Zero_Page_Y_Indexed();
    test_LDX_Absolute();
    test_LDX_Absolute_Y_Indexed();
    test_LDX_Absolute_Y_Indexed_Boundary_Cross();

    puts("--- All LDX tests completed successfully.");
}

void test_LDX_Immediate(void)
{
    puts("+ Testing LDX [ 0xA2 ], immediate addressing mode");
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

void test_LDX_Zero_Page(void)
{
    puts("+ Testing LDX [ 0xA6 ], zero page addressing mode");
    RESET()
    uint8_t data = 0;

    /* Setup the test by pre-loading our test data into memory */
    insert_test_data(0x0081, 0xAA);

    /* Now load our test program and start clocking the CPU */
    cartridge_load(test_cart_LDX_Zero_Page);
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Fetch the value from memory and load it */
    /* End test */

    /* Do we have the expected result (0xAA) in the X register? */
    mos6507_get_register(MOS6507_REG_X, &data);
    assert(data == 0xAA);
}


void test_LDX_Zero_Page_Y_Indexed(void)
{
    puts("+ Testing LDX [ 0xB6 ], zero page Y indexed addressing mode");
    RESET()
    uint8_t data = 0;

    /* Setup the test by pre-loading our test data into memory */
    insert_test_data(0x0091, 0xAA);

    /* The cartridge specifies 0x90, this offset gets us to the correct location */
    mos6507_set_register(MOS6507_REG_Y, 0x01);

    /* Now load our test program and start clocking the CPU */
    cartridge_load(test_cart_LDX_Zero_Page_Y_Indexed);
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Fetch the value from memory and use it as the next address */
    mos6507_clock_tick(); /* Fetch the X register and index it with the address */
    /* End test */

    /* Do we have the expected result (0xAA) in the X register? */
    mos6507_get_register(MOS6507_REG_X, &data);
    assert(data == 0xAA);
}

void test_LDX_Absolute(void)
{
    puts("+ Testing LDX [ 0xAE ], absolute addressing mode");
    RESET()
    uint8_t data = 0;

    /* Setup the test by pre-loading our test data into memory */
    insert_test_data(0x2001, 0xAA);

    /* Now load our test program and start clocking the CPU */
    cartridge_load(test_cart_LDX_Absolute);
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the low memory location */
    mos6507_clock_tick(); /* Fetch the next byte for the high memory location */
    mos6507_clock_tick(); /* Fetch the value from memory and load it */
    /* End test */

    /* Do we have the expected result (0xAA) in the X register? */
    mos6507_get_register(MOS6507_REG_X, &data);
    assert(data == 0xAA);
}

void test_LDX_Absolute_Y_Indexed(void)
{
    puts("+ Testing LDX [ 0xBE ], absolute Y indexed addressing mode");
    RESET()
    uint8_t data = 0;

    /* Setup the test by pre-loading our test data into memory */
    insert_test_data(0x2001, 0xAA);

    /* The cartridge specifies 0x2000, this offset gets us to the correct location */
    mos6507_set_register(MOS6507_REG_Y, 0x01);

    /* Now load our test program and start clocking the CPU */
    cartridge_load(test_cart_LDX_Absolute_Y_Indexed);
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the low memory location */
    mos6507_clock_tick(); /* Fetch the next byte for the high memory location */
    mos6507_clock_tick(); /* Fetch the value from memory and load it */
    /* End test */

    /* Do we have the expected result (0xAA) in the X register? */
    mos6507_get_register(MOS6507_REG_X, &data);
    assert(data == 0xAA);
}

void test_LDX_Absolute_Y_Indexed_Boundary_Cross(void)
{
    puts("+ Testing LDX [ 0xBE ], absolute Y indexed addressing mode with boundary crossing");
    RESET()
    uint8_t data = 0;

    /* Setup the test by pre-loading our test data into memory */
    insert_test_data(0x0100, 0xAA);

    /* The cartridge specifies 0x2000, this offset gets us to the correct location */
    mos6507_set_register(MOS6507_REG_Y, 0x01);

    /* Now load our test program and start clocking the CPU */
    cartridge_load(test_cart_LDX_Absolute_Y_Indexed_Boundary_Cross);
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the low memory location */
    mos6507_clock_tick(); /* Fetch the next byte for the high memory location */
    mos6507_clock_tick(); /* Compute new address with carry */
    mos6507_clock_tick(); /* Fetch the value from memory and load it */
    /* End test */

    /* Do we have the expected result (0xAA) in the X register? */
    mos6507_get_register(MOS6507_REG_X, &data);
    assert(data == 0xAA);
}

/******************************************************************************
 * Load the Y register
 *****************************************************************************/

void test_LDY(void)
{
    puts("--- Testing LDY:");

    test_LDY_Immediate();
    test_LDY_Zero_Page();
    test_LDY_Zero_Page_X_Indexed();
    test_LDY_Absolute();
    test_LDY_Absolute_X_Indexed();
    test_LDY_Absolute_X_Indexed_Boundary_Cross();

    puts("--- All LDY tests completed successfully.");
}

void test_LDY_Immediate(void)
{
    puts("+ Testing LDY [ 0xA0 ], immediate addressing mode");
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

void test_LDY_Zero_Page(void)
{
    puts("+ Testing LDY [ 0xA4 ], zero page addressing mode");
    RESET()
    uint8_t data = 0;

    /* Setup the test by pre-loading our test data into memory */
    insert_test_data(0x0081, 0xAA);

    /* Now load our test program and start clocking the CPU */
    cartridge_load(test_cart_LDY_Zero_Page);
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Fetch the value from memory and load it */
    /* End test */

    /* Do we have the expected result (0xAA) in the Y register? */
    mos6507_get_register(MOS6507_REG_Y, &data);
    assert(data == 0xAA);
}


void test_LDY_Zero_Page_X_Indexed(void)
{
    puts("+ Testing LDY [ 0xB4 ], zero page X indexed addressing mode");
    RESET()
    uint8_t data = 0;

    /* Setup the test by pre-loading our test data into memory */
    insert_test_data(0x0091, 0xAA);

    /* The cartridge specifies 0x90, this offset gets us to the correct location */
    mos6507_set_register(MOS6507_REG_X, 0x01);

    /* Now load our test program and start clocking the CPU */
    cartridge_load(test_cart_LDY_Zero_Page_X_Indexed);
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Fetch the value from memory and use it as the next address */
    mos6507_clock_tick(); /* Fetch the X register and index it with the address */
    /* End test */

    /* Do we have the expected result (0xAA) in the Y register? */
    mos6507_get_register(MOS6507_REG_Y, &data);
    assert(data == 0xAA);
}

void test_LDY_Absolute(void)
{
    puts("+ Testing LDY [ 0xAC ], absolute addressing mode");
    RESET()
    uint8_t data = 0;

    /* Setup the test by pre-loading our test data into memory */
    insert_test_data(0x2001, 0xAA);

    /* Now load our test program and start clocking the CPU */
    cartridge_load(test_cart_LDY_Absolute);
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the low memory location */
    mos6507_clock_tick(); /* Fetch the next byte for the high memory location */
    mos6507_clock_tick(); /* Fetch the value from memory and load it */
    /* End test */

    /* Do we have the expected result (0xAA) in the Y register? */
    mos6507_get_register(MOS6507_REG_Y, &data);
    assert(data == 0xAA);
}

void test_LDY_Absolute_X_Indexed(void)
{
    puts("+ Testing LDX [ 0xBC ], absolute X indexed addressing mode");
    RESET()
    uint8_t data = 0;

    /* Setup the test by pre-loading our test data into memory */
    insert_test_data(0x2001, 0xAA);

    /* The cartridge specifies 0x2000, this offset gets us to the correct location */
    mos6507_set_register(MOS6507_REG_X, 0x01);

    /* Now load our test program and start clocking the CPU */
    cartridge_load(test_cart_LDY_Absolute_X_Indexed);
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the low memory location */
    mos6507_clock_tick(); /* Fetch the next byte for the high memory location */
    mos6507_clock_tick(); /* Fetch the value from memory and load it */
    /* End test */

    /* Do we have the expected result (0xAA) in the Y register? */
    mos6507_get_register(MOS6507_REG_Y, &data);
    assert(data == 0xAA);
}

void test_LDY_Absolute_X_Indexed_Boundary_Cross(void)
{
    puts("+ Testing LDY [ 0xBC ], absolute X indexed addressing mode with boundary crossing");
    RESET()
    uint8_t data = 0;

    /* Setup the test by pre-loading our test data into memory */
    insert_test_data(0x0100, 0xAA);

    /* The cartridge specifies 0x2000, this offset gets us to the correct location */
    mos6507_set_register(MOS6507_REG_X, 0x01);

    /* Now load our test program and start clocking the CPU */
    cartridge_load(test_cart_LDY_Absolute_X_Indexed_Boundary_Cross);
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the low memory location */
    mos6507_clock_tick(); /* Fetch the next byte for the high memory location */
    mos6507_clock_tick(); /* Compute new address with carry */
    mos6507_clock_tick(); /* Fetch the value from memory and load it */
    /* End test */

    /* Do we have the expected result (0xAA) in the Y register? */
    mos6507_get_register(MOS6507_REG_Y, &data);
    assert(data == 0xAA);
}

/******************************************************************************
 * Store the Accumulator
 *****************************************************************************/

void test_STA(void)
{
    puts("--- Testing STA:");

    test_STA_Zero_Page();
    test_STA_Zero_Page_X_Indexed();
    test_STA_Absolute();
    test_STA_Absolute_X_Indexed();
    test_STA_Absolute_Y_Indexed();
    test_STA_Indirect_X_Indexed();
    test_STA_Indirect_Y_Indexed();

    puts("--- All STA tests completed successfully.");
}

void test_STA_Zero_Page(void)
{
    puts("+ Testing STA [ 0xA9 ], zero page addressing mode");
    RESET()
    uint8_t data = 0;

    /* Now load our test program and start clocking the CPU */
    cartridge_load(test_cart_STA_Zero_Page);
    /* Load (LDA) a value into the Accumulator */
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Fetch the value from memory and load it */
    /* Now execute the store (STA) function */
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Store the Accumulator value into memory */
    /* End test */

    /* Do we have the expected result (0xBB) in memory? */
    read_test_data(0x0099, &data);
    assert(data == 0xBB);
}

void test_STA_Zero_Page_X_Indexed(void)
{
    puts("+ Testing STA [ 0x95 ], zero page X indexed addressing mode");
    RESET()
    uint8_t data = 0;

    /* Set out offset to be added to the address in the cartridge */
    mos6507_set_register(MOS6507_REG_X, 0x09);

    /* Now load our test program and start clocking the CPU */
    cartridge_load(test_cart_STA_Zero_Page_X_Indexed);
    /* Load (LDA) a value into the Accumulator */
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Fetch the value from memory and load it */
    /* Now execute the store (STA) function */
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Update the address bus to the memory address + index */
    mos6507_clock_tick(); /* Store the Accumulator value into memory */
    /* End test */

    /* Do we have the expected result (0xBB) in memory? */
    read_test_data(0x0099, &data);
    assert(data == 0xBB);
}

void test_STA_Absolute(void)
{
    puts("+ Testing STA [ 0x8D ], absolute addressing mode");
    RESET()
    uint8_t data = 0;

    /* Now load our test program and start clocking the CPU */
    cartridge_load(test_cart_STA_Absolute);
    /* Load (LDA) a value into the Accumulator */
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Fetch the value from memory and load it */
    /* Now execute the store (STA) function */
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Update the address bus to the memory address */
    mos6507_clock_tick(); /* Store the Accumulator value into memory */
    /* End test */

    /* Do we have the expected result (0xBB) in memory? */
    read_test_data(0x01FF, &data);
    assert(data == 0xBB);
}

void test_STA_Absolute_X_Indexed(void)
{
    puts("+ Testing STA [ 0x9D ], absolute X indexed addressing mode");
    RESET()
    uint8_t data = 0;

    /* Set out offset to be added to the address in the cartridge */
    mos6507_set_register(MOS6507_REG_X, 0x0F);

    /* Now load our test program and start clocking the CPU */
    cartridge_load(test_cart_STA_Absolute_X_Indexed);
    /* Load (LDA) a value into the Accumulator */
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Fetch the value from memory and load it */
    /* Now execute the store (STA) function */
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Update the address bus to the memory address + index */
    mos6507_clock_tick(); /* Set busses */
    mos6507_clock_tick(); /* Store the Accumulator value into memory */
    /* End test */

    /* Do we have the expected result (0xBB) in memory? */
    read_test_data(0x01FF, &data);
    assert(data == 0xBB);
}

void test_STA_Absolute_Y_Indexed(void)
{
    puts("+ Testing STA [ 0x99 ], absolute Y indexed addressing mode");
    RESET()
    uint8_t data = 0;

    /* Set out offset to be added to the address in the cartridge */
    mos6507_set_register(MOS6507_REG_Y, 0x0F);

    /* Now load our test program and start clocking the CPU */
    cartridge_load(test_cart_STA_Absolute_Y_Indexed);
    /* Load (LDA) a value into the Accumulator */
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Fetch the value from memory and load it */
    /* Now execute the store (STA) function */
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Update the address bus to the memory address + index */
    mos6507_clock_tick(); /* Set busses */
    mos6507_clock_tick(); /* Store the Accumulator value into memory */
    /* End test */

    /* Do we have the expected result (0xBB) in memory? */
    read_test_data(0x01FF, &data);
    assert(data == 0xBB);
}

void test_STA_Indirect_X_Indexed(void)
{
    puts("+ Testing STA [ 0x81 ], indirect X indexed addressing mode");
    RESET()
    uint8_t data = 0;

    /* Setup the test by pre-loading our test data into memory */
    insert_test_data(0x0090, 0x90);
    insert_test_data(0x0095, 0xFF);
    insert_test_data(0x0096, 0x01);

    /* Set out offset to be added to the address in the cartridge */
    mos6507_set_register(MOS6507_REG_X, 0x05);

    /* Now load our test program and start clocking the CPU */
    cartridge_load(test_cart_STA_Indirect_X_Indexed);
    /* Load (LDA) a value into the Accumulator */
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Fetch the value from memory and load it */
    /* Now execute the store (STA) function */
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Update the address bus to the memory address + index */
    mos6507_clock_tick(); /* Set busses */
    mos6507_clock_tick(); /* Set busses */
    mos6507_clock_tick(); /* Store the Accumulator value into memory */
    /* End test */

    /* Do we have the expected result (0xBB) in memory? */
    read_test_data(0x01FF, &data);
    assert(data == 0xBB);
}

void test_STA_Indirect_Y_Indexed(void)
{
    puts("+ Testing STA [ 0x91 ], indirect Y indexed addressing mode");
    RESET()
    uint8_t data = 0;

    /* Setup the test by pre-loading our test data into memory */
    insert_test_data(0x0090, 0x90);
    insert_test_data(0x0095, 0xFF);
    insert_test_data(0x0096, 0x01);

    /* Set out offset to be added to the address in the cartridge */
    mos6507_set_register(MOS6507_REG_Y, 0x05);

    /* Now load our test program and start clocking the CPU */
    cartridge_load(test_cart_STA_Indirect_Y_Indexed);
    /* Load (LDA) a value into the Accumulator */
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Fetch the value from memory and load it */
    /* Now execute the store (STA) function */
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Update the address bus to the memory address + index */
    mos6507_clock_tick(); /* Set busses */
    mos6507_clock_tick(); /* Set busses */
    mos6507_clock_tick(); /* Store the Accumulator value into memory */
    /* End test */

    /* Do we have the expected result (0xBB) in memory? */
    read_test_data(0x01FF, &data);
    assert(data == 0xBB);
}

/******************************************************************************
 * Store the X index register
 *****************************************************************************/

void test_STX(void)
{
    puts("--- Testing STX:");

    test_STX_Zero_Page();
    test_STX_Zero_Page_Y_Indexed();
    test_STX_Absolute();

    puts("--- All STX tests completed successfully.");
}

void test_STX_Zero_Page(void)
{
    puts("+ Testing STX [ 0x86 ], zero page addressing mode");
    RESET()
    uint8_t data = 0;

    /* Now load our test program and start clocking the CPU */
    cartridge_load(test_cart_STX_Zero_Page);
    /* Load (LDX) a value into the X index register */
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Fetch the value from memory and load it */
    /* Now execute the store (STX) function */
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Store the X index value into memory */
    /* End test */

    /* Do we have the expected result (0xBB) in memory? */
    read_test_data(0x0099, &data);
    assert(data == 0xBB);
}

void test_STX_Zero_Page_Y_Indexed(void)
{
    puts("+ Testing STX [ 0x96 ], zero page Y indexed addressing mode");
    RESET()
    uint8_t data = 0;

    /* Set out offset to be added to the address in the cartridge */
    mos6507_set_register(MOS6507_REG_Y, 0x09);

    /* Now load our test program and start clocking the CPU */
    cartridge_load(test_cart_STX_Zero_Page_Y_Indexed);
    /* Load (LDX) a value into the X index register */
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Fetch the value from memory and load it */
    /* Now execute the store (STX) function */
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Update the address bus to the memory address + index */
    mos6507_clock_tick(); /* Store the X index value into memory */
    /* End test */

    /* Do we have the expected result (0xBB) in memory? */
    read_test_data(0x0099, &data);
    assert(data == 0xBB);
}

void test_STX_Absolute(void)
{
    puts("+ Testing STX [ 0x8E ], absolute addressing mode");
    RESET()
    uint8_t data = 0;

    /* Now load our test program and start clocking the CPU */
    cartridge_load(test_cart_STX_Absolute);
    /* Load (LDX) a value into the X index register */
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Fetch the value from memory and load it */
    /* Now execute the store (STX) function */
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Store the X index value into memory */
    /* End test */

    /* Do we have the expected result (0xBB) in memory? */
    read_test_data(0x01FF, &data);
    assert(data == 0xBB);
}

/******************************************************************************
 * Store the Y index register
 *****************************************************************************/

void test_STY(void)
{
    puts("--- Testing STY:");

    test_STY_Zero_Page();
    test_STY_Zero_Page_X_Indexed();
    test_STY_Absolute();

    puts("--- All STY tests completed successfully.");
}

void test_STY_Zero_Page(void)
{
    puts("+ Testing STY [ 0x84 ], zero page addressing mode");
    RESET()
    uint8_t data = 0;

    /* Now load our test program and start clocking the CPU */
    cartridge_load(test_cart_STY_Zero_Page);
    /* Load (LDY) a value into the Y index register */
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Fetch the value from memory and load it */
    /* Now execute the store (STY) function */
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Store the Y index value into memory */
    /* End test */

    /* Do we have the expected result (0xBB) in memory? */
    read_test_data(0x0099, &data);
    assert(data == 0xBB);
}

void test_STY_Zero_Page_X_Indexed(void)
{
    puts("+ Testing STY [ 0x94 ], zero page X indexed addressing mode");
    RESET()
    uint8_t data = 0;

    /* Set out offset to be added to the address in the cartridge */
    mos6507_set_register(MOS6507_REG_X, 0x09);

    /* Now load our test program and start clocking the CPU */
    cartridge_load(test_cart_STY_Zero_Page_X_Indexed);
    /* Load (LDY) a value into the Y index register */
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Fetch the value from memory and load it */
    /* Now execute the store (STY) function */
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Update the address bus to the memory address + index */
    mos6507_clock_tick(); /* Store the Y index value into memory */
    /* End test */

    /* Do we have the expected result (0xBB) in memory? */
    read_test_data(0x0099, &data);
    assert(data == 0xBB);
}

void test_STY_Absolute(void)
{
    puts("+ Testing STY [ 0x8C ], absolute addressing mode");
    RESET()
    uint8_t data = 0;

    /* Now load our test program and start clocking the CPU */
    cartridge_load(test_cart_STY_Absolute);
    /* Load (LDY) a value into the Y index register */
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Fetch the value from memory and load it */
    /* Now execute the store (STY) function */
    mos6507_clock_tick(); /* Read the instruction */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Fetch the next byte for the memory location */
    mos6507_clock_tick(); /* Store the Y index value into memory */
    /* End test */

    /* Do we have the expected result (0xBB) in memory? */
    read_test_data(0x01FF, &data);
    assert(data == 0xBB);
}

/******************************************************************************
 * Add to Accumulator with carry check
 *****************************************************************************/

void test_ADC(void)
{
    puts("--- Testing ADC:");

    test_ADC_Immediate();

    puts("--- All ADC tests completed successfully.");

}

void test_ADC_Immediate(void)
{
    puts("+ Testing ADC [ 0x69 ], immediate addressing mode");
    RESET()
    uint8_t accumulator, P = 0;

    /* Do we have the expected result in the Accumulator and 
     * has the Carry bit been set correctly? */
    mos6507_get_register(MOS6507_REG_S, &P);
    assert(MOS6507_STATUS_FLAG_CARRY & P);

    mos6507_get_register(MOS6507_REG_A, &accumulator);
    assert(accumulator = 10);
}

#endif /* EXEC_TESTS */
