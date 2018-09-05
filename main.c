/* See LICENSE file for license details */

/* Standard library includes */
#include <stdio.h>
#include <string.h>

/* HiFive1/FE310 includes */
#include "encoding.h"
#include "platform.h"
#include "plic/plic_driver.h"

/* Custom HiFive1 includes*/
#include "external/spi.h"
#include "external/UART_driver.h"

/* Atari and platform includes */
#include "mos6507/mos6507.h"
#include "atari/Atari-TIA.h"
#include "atari/Atari-cart.h"
#include "mos6532/mos6532.h"
#ifdef EXEC_TESTS
    #include "test/test-carts.h"
    #include "test/tests.h"
#endif
#ifdef PRINT_STATE
    #include "test/debug.h"
#endif
/* Game cart data */
#include "carts/halo2600.h"

/* Calculation assumes a core frequency of ~262MHz and pwmscale of 1.
 * Integer value is a result of:
 *
 * (Clock source / divisor) / FREQUENCY
 *
 * Eg., 3.58MHz TIA input clock::
 *
 * (262MHz / 2^1) / 3580000 = 36 (0x24 in hex)
 */
#ifndef STANDALONE
#define PWM_FREQ        0x24
#define PWM_FREQ_DIV    0x0C /* PWM_FREQ/2 for 50% duty */
#define PWM_SCALE       0x01
#else
/* When not expecting a slave TIA device to scale the system clock
 * generate our own. 6507 input clock is 3.58MHz/3:
 *
 * (262MHz / 2^1) / 1193333 = ~110 (0x6E in hex)
 */
#define PWM_FREQ        0x6E
#define PWM_FREQ_DIV    0x37 /* PWM_FREQ/2 for 50% duty */
#define PWM_SCALE       0x01
#endif /* STANDALONE */

#if 0
/*
 * (300MHz / 2^1) / 3580000 = ~42 (0x2A in hex)
 */
#define PWM_FREQ        0x2A
#define PWM_FREQ_DIV    0x15 /* PWM_FREQ/2 for 50% duty */
#define PWM_SCALE       0x01
#endif

#define CLOCK_INPUT PIN_19_OFFSET

/* I/O masks */
static const uint32_t BLUE_LED_MASK    = (0x1 << BLUE_LED_OFFSET);
static const uint32_t GREEN_LED_MASK   = (0x1 << GREEN_LED_OFFSET);
static const uint32_t CLOCK_INPUT_MASK = (0x1 << CLOCK_INPUT);

/* Globals */
static const char atari_logo[] = "\n\r"
"\n\r"
"             HiFive1-2600\n\r"
"             ------------\n\r"
"\n\r"
"         by David Grubb, 2017\n\r"
"  https://github.com/dgrubb/HiFive1-2600\n\r"
"\n\r"
"An Atari 2600 emulator for an IoT platform.\n\r"
"\n\r"

"              $$ $$$$$ $$\n\r"
"              $$ $$$$$ $$\n\r"
"             .$$ $$$$$ $$.\n\r"
"             :$$ $$$$$ $$:\n\r"
"             $$$ $$$$$ $$$\n\r"
"             $$$ $$$$$ $$$\n\r"
"            ,$$$ $$$$$ $$$.\n\r"
"           ,$$$$ $$$$$ $$$$.\n\r"
"          ,$$$$; $$$$$ :$$$$.\n\r"
"         ,$$$$$  $$$$$  $$$$$.\n\r"
"       ,$$$$$$'  $$$$$  `$$$$$$.\n\r"
"     ,$$$$$$$'   $$$$$   `$$$$$$$.\n\r"
"  ,s$$$$$$$'     $$$$$     `$$$$$$$s.\n\r"
"$$$$$$$$$'       $$$$$       `$$$$$$$$$\n\r"
"$$$$$Y'          $$$$$          `Y$$$$$\n\r"
"\n\r"
"            Welcome to 1977!\n\r";

/******************************************************************************
 * Interrupt callbacks
 *****************************************************************************/

void handle_m_time_interrupt()
{
}

void handle_m_ext_interrupt()
{
}

/******************************************************************************
 * Hardware initialisation
 *****************************************************************************/

void init_clock()
{
    /* Clear PWM configuration register */
    PWM1_REG(PWM_CFG) = 0;

    /* This is the real meat of things. PWM configuration register bits are
     * documented in the SiFive E300 Platform Reference Manual:
     *
     * https://www.sifive.com/documentation/freedom-soc/freedom-e300-platform-reference-manual/
     */
    PWM1_REG(PWM_CFG) =
        (PWM_CFG_DEGLITCH)   |
        (PWM_CFG_CMP2CENTER) |
        (PWM_CFG_CMP3CENTER) |
        (PWM_CFG_ZEROCMP)    |
        (PWM_SCALE);
    PWM1_REG(PWM_CMP0) = PWM_FREQ;
    PWM1_REG(PWM_CMP3) = PWM_FREQ_DIV; /* Output a 50% duty cycle */

    /* Reset the PWM count register, ready for usage */
    PWM1_REG(PWM_COUNT) = 0;
}

void init_GPIO ()
{
    /* Debug output indicator */
    GPIO_REG(GPIO_OUTPUT_EN)   |=   BLUE_LED_MASK;
    GPIO_REG(GPIO_OUTPUT_VAL)  &=  ~BLUE_LED_MASK;

    /* Clock source ouput (PWM) */
    GPIO_REG(GPIO_IOF_SEL)     |=   GREEN_LED_MASK;
    GPIO_REG(GPIO_IOF_EN)      |=   GREEN_LED_MASK;
}

void enable_interrupts()
{
    set_csr(mie, MIP_MEIP);
    set_csr(mie, MIP_MTIP);
    set_csr(mstatus, MSTATUS_MIE);
}

/******************************************************************************
 * Main code entry point
 *****************************************************************************/

int main()
{
    /* Display Atari's awesome logo */
    puts(atari_logo);

    /* Setup and reset all the emulated
     * hardware: memory, CPU, TIA etc ...
     */
    opcode_populate_ISA_table();
    mos6532_clear_memory();
    TIA_init();

    /* Emulation is ready to start so load cartridge and reset CPU */
    cartridge_load(Halo_2600);
    mos6507_reset();

    /* Setup FE310 peripherals */
    init_GPIO();
    UART_init(115200, 0);
    init_clock();
    enable_interrupts();

#ifdef MANUAL_STEP
    /* Executes a cartridge as normal, but instead of waiting on clock signal
     * the program executes a clock per key press on the UART. While in this
     * mode the UART output will print status register and current execution
     * state to a human-readable output. The intention is to allow for loading
     * a proper Atari ROM dump and step through it manually.
     */
    char wait;
    while (1) {
        /* UART_get_char() has a blocking option, but we don't really care
         * what key was pressed.
         */
        UART_get_char(&wait, 1);
        GPIO_REG(GPIO_OUTPUT_VAL) ^=  BLUE_LED_MASK;
        mos6507_clock_tick();
    };
#endif /* MANUAL_STEP */

#ifdef EXEC_TESTS
    /* Program flow is different when testing the consistency of the 6502
     * state model. Rather than awaiting an external clock tick we simply step
     * through automatically. Exit with the test result once we're finished.
     */
    execute_tests();
    return 0;
#endif /* EXEC_TESTS */

    /* This is the intended use-case, full speed clock source running a
     * proper cart image.
     */
    while (1) {
        GPIO_REG(GPIO_OUTPUT_VAL)  ^=  BLUE_LED_MASK;
        PWM1_REG(PWM_CFG) |= PWM_CFG_ONESHOT;
        /* Fire a CPU clock tick */
        if (mos6507_clock_tick()) {
            /* There was a significant and unrecoverable problem executing
             * the current ROM, breakout here
             */
            break;
        }
        while (PWM1_REG(PWM_COUNT)) {}
    };

    return 0;
}
