/* See LICENSE file for license details */

/* Standard library includes */
#include <stdio.h>
#include <string.h>

/* HiFive1/FE310 includes */
#include "encoding.h"
#include "platform.h"
#include "plic/plic_driver.h"

/* Atari and platform includes */
#include "cpu/mos6507.h"
#include "external/spi.h"
#include "atari/Atari-TIA.h"
#include "memory/mos6532.h"
#include "test/tests.h"

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
#define PWM_FREQ    0x24
#define PWM_SCALE   0x01
#else
/* When not expecting a slave TIA device to scale the system clock
 * generate our own. 6507 input clock is 3.58MHz/3:
 *
 * (262MHz / 2^1) / 1193333 = ~110 (0x6E in hex)
 */
#define PWM_FREQ    0x6E
#define PWM_SCALE   0x01
#endif /* STANDALONE */

#define CLOCK_INPUT PIN_19_OFFSET

/* I/O masks */
static const uint32_t BLUE_LED_MASK    = (0x1 << BLUE_LED_OFFSET);
static const uint32_t GREEN_LED_MASK   = (0x1 << GREEN_LED_OFFSET);
static const uint32_t CLOCK_INPUT_MASK = (0x1 << CLOCK_INPUT);

/* Globals */
static plic_instance_t g_plic;
volatile uint8_t system_clock = 0;
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
    puts("Timer interrupt");
}

void handle_m_ext_interrupt()
{
    plic_source int_num = PLIC_claim_interrupt(&g_plic);
    switch (int_num) {
        case PIN_19_OFFSET:
            GPIO_REG(GPIO_RISE_IP) = (0x1 << CLOCK_INPUT);
            system_clock = 1;
            break;
    }
    PLIC_complete_interrupt(&g_plic, int_num);
}

/******************************************************************************
 * Hardware initialisation
 *****************************************************************************/

void init_clock()
{
    /* Setup interrupt using PWM1 as the source */
    PLIC_init(&g_plic, PLIC_CTRL_ADDR, PLIC_NUM_INTERRUPTS, PLIC_NUM_PRIORITIES);
    PLIC_enable_interrupt(&g_plic, INT_PWM1_BASE);
    PLIC_set_threshold(&g_plic, 0);
    PLIC_set_priority(&g_plic, INT_PWM1_BASE, 1);

    /* Clear PWM configuration register */
    PWM1_REG(PWM_CFG) = 0;

    /* This is the real meat of things. PWM configuration register bits are
     * documented in the SiFive E300 Platform Reference Manual:
     *
     * https://www.sifive.com/documentation/freedom-soc/freedom-e300-platform-reference-manual/
     */
    PWM1_REG(PWM_CFG) =
        (PWM_CFG_ENALWAYS)   |
        (PWM_CFG_DEGLITCH)   |
        (PWM_CFG_CMP2CENTER) |
        (PWM_CFG_CMP3CENTER) |
        (PWM_CFG_ZEROCMP)    |
        (PWM_SCALE);
    PWM1_REG(PWM_COUNT) = 0;
    PWM1_REG(PWM_CMP0) = PWM_FREQ;
    PWM1_REG(PWM_CMP3) = PWM_FREQ/2; /* Output a 50% duty cycle */

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

    /* Clock source input */
    GPIO_REG(GPIO_OUTPUT_EN)   &=  ~(0x1 << (CLOCK_INPUT - INT_GPIO_BASE));
    GPIO_REG(GPIO_PULLUP_EN)   &=  ~(0x1 << (CLOCK_INPUT - INT_GPIO_BASE));
    GPIO_REG(GPIO_INPUT_EN)    |=   (0x1 << (CLOCK_INPUT - INT_GPIO_BASE));
    GPIO_REG(GPIO_RISE_IE)     |=   (0x1 << (CLOCK_INPUT - INT_GPIO_BASE));
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
    mos6507_reset();
    TIA_init();

    /* Setup FE310 peripherals */
    init_GPIO();
    init_SPI();
    init_clock();
    enable_interrupts();

#ifdef EXEC_TESTS
    /* Program flow is different when testing the consistency of the 6502
     * state model. Rather than awaiting an external clock tick we simply step
     * through automatically. Exit with the test result once we're finished.
     */
    execute_tests();
    return 0;
#endif /* EXEC_TESTS */

    while (1) {
        if(system_clock) {
            GPIO_REG(GPIO_OUTPUT_VAL)  ^=  BLUE_LED_MASK;
            /* Fire a CPU clock tick */
            mos6507_clock_tick();
            /* Trasmit any changes in data and address bus to peripherals */
            spi_transmit_bus_states();
            system_clock = 0;
        }
    };

    return 0;
}

