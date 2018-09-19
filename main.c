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
#include "carts/kernel_13.h"

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
#ifdef MANUAL_STEP
    char wait;
#endif /* MANUAL_STEP*/

    /* Display Atari's awesome logo */
    puts(atari_logo);

    /* Setup and reset all the emulated
     * hardware: memory, CPU, TIA etc ...
     */
    opcode_populate_ISA_table();
    mos6532_clear_memory();
    TIA_init();

    /* Emulation is ready to start so load cartridge and reset CPU */
    cartridge_load(kernel_13);
    mos6507_reset();

    /* Setup FE310 peripherals */
    init_GPIO();
    UART_init(115200, 0);
    init_clock();
    init_timer();
    init_SPI();
    ili9341_init();
    enable_interrupts();

#ifdef EXEC_TESTS
    execute_tests();
    return 0;
#endif /* EXEC_TESTS */


#ifdef COLOUR_TEST
    colour_test();
    return 0;
#endif /* COLOUR_TEST */

    /* This is the intended use-case, full speed clock source running a
     * proper cart image.
     */
    uint32_t vblank = 0;
    uint32_t vsync = 0;
    uint32_t line_count = 0;
    while(1) {
        GPIO_REG(GPIO_OUTPUT_VAL)  ^=  BLUE_LED_MASK;
#ifdef MANUAL_STEP
        UART_get_char(&wait, 1);
#else
        PWM1_REG(PWM_CFG) |= PWM_CFG_ONESHOT;
#endif
        if (raster_line()) {
            /* Error in emulation encountered */
            goto end;
        }
#ifdef MANUAL_STEP
#else
        while (PWM1_REG(PWM_COUNT)) {}
#endif
        if (vsync && !TIA_get_VSYNC()) {
            line_count = 0;
            vblank = TIA_VERTICAL_BLANK_LINES;
        }
        vsync = TIA_get_VSYNC();
        if (!vsync && !vblank && (line_count < TIA_VERTICAL_PICTURE_LINES)) {
            TIA_draw_line(line_count);
            TIA_reset_buffer();
            line_count++;
        }
        if (vblank) {
            vblank--;
        }
    }

end: ;

    return 0;
}
