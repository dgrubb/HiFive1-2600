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
#include "external/ili9341.h"
#include "external/platform_util.h"

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
 * Main code entry point
 *****************************************************************************/

int main()
{
    int i;
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
    init_timer();
    init_SPI();
    ili9341_init();
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


#ifdef COLOUR_TEST
    int line_count=0;
    while(1) {
        for (line_count=0; line_count<192; line_count++) {
            ili9341_draw_line(tia_test_line, line_count, 160);
        }
    }
#endif /* COLOUR_TEST */

    /* This is the intended use-case, full speed clock source running a
     * proper cart image.
     */
    while (1) {
        GPIO_REG(GPIO_OUTPUT_VAL)  ^=  BLUE_LED_MASK;
        PWM1_REG(PWM_CFG) |= PWM_CFG_ONESHOT;
        for (i=0; i<TIA_COLOUR_CLOCK_TOTAL; i++) {
            TIA_clock_tick();
            /* Fire a CPU clock tick */
            if (!TIA_get_WSYNC()) {
                if (mos6507_clock_tick()) {
                    /* There was a significant and unrecoverable problem executing
                     * the current ROM, breakout here
                     */
                    goto end;
                }
            } else {
                puts("WSYNC\n\r");
            }
        }
        while (PWM1_REG(PWM_COUNT)) {}
        // TODO: after line timeout send data over SPI to screen
    };

end: ;

    return 0;
}
