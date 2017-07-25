/* See LICENSE file for license details */

/* Standard library includes */
#include <stdio.h>
#include <string.h>

/* HiFive1/FE310 includes */
#include "encoding.h"
#include "platform.h"
#include "plic/plic_driver.h"

/* Atari includes */
#include "display.h"
#include "Atari-TIA.h"
#include "mos6507.h"
#include "mos6532.h"

/* Calculation assumes a core frequency of ~262MHz and pwmscale of 1.
 * Integer value is a result of:
 *
 * (Clock source / divisor) / FREQUENCY
 *
 * Eg., 3.58MHz TIA input clock::
 *
 * (262MHz / 2^1) / 3580000 = 36
 */
#define PWM_FREQ  0x24
#define PWM_SCALE 0x01

static const uint32_t BLUE_LED_MASK = (0x1 << BLUE_LED_OFFSET);
static plic_instance_t g_plic;
volatile uint8_t TIA_clock = 0;
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

void handle_m_time_interrupt()
{
    puts("Timer interrupt");
}

void handle_m_ext_interrupt()
{
    plic_source int_num = PLIC_claim_interrupt(&g_plic);
    switch (int_num) {
        case 0:
            break;
        case INT_PWM1_BASE:
            PWM1_REG(PWM_CFG) &= ~PWM_CFG_CMP0IP;
            TIA_clock = 1;
            break;
    }
    PLIC_complete_interrupt(&g_plic, int_num);
}

void init_TIA_clock()
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
        (PWM_CFG_ENALWAYS) |
        (PWM_CFG_ZEROCMP)  |
        (PWM_CFG_STICKY)   |
        (PWM_SCALE);
    PWM1_REG(PWM_CMP0) = PWM_FREQ;

    /* Reset the PWM count register, ready for usage */
    PWM1_REG(PWM_COUNT) = 0;
}

void init_GPIO ()
{
    GPIO_REG(GPIO_OUTPUT_EN)   |=  BLUE_LED_MASK;
    GPIO_REG(GPIO_OUTPUT_VAL)  &=  ~BLUE_LED_MASK;
}

void enable_interrupts()
{
    set_csr(mie, MIP_MEIP);
    set_csr(mie, MIP_MTIP);
    set_csr(mstatus, MSTATUS_MIE);
}

int main()
{
    uint8_t CPU_clock = 0;

    /* Display Atari's awesome logo */
    puts(atari_logo);

    /* Setup FE310 peripherals */
    display_init();

    init_GPIO();
    /* Setup and reset all the emulated
     * hardware: memory, CPU, TIA etc ...
     */
    opcode_populate_ISA_table();
    TIA_init();
    mos6532_clear_memory();
    mos6507_reset();

    /* The TIA's clock is actually the fastest on the system at
     * 3.58MHz. The CPU derives its clock from the TIA and receives
     * a clock tick every third TIA tick, translating to a CPU clock
     * of 1.19MHz.
     */
    init_TIA_clock();

    enable_interrupts();

    while (1) {
        if(TIA_clock) {
            GPIO_REG(GPIO_OUTPUT_VAL)  ^=  BLUE_LED_MASK;
            /* Handle TIA and CPU clocks */
            TIA_clock = 0;
            CPU_clock++;

            /* Now execute hardware ticks */
            TIA_clock_tick();
            if (CPU_clock >= 3) {
                CPU_clock = 0;
                mos6507_clock_tick();
            }
        }
    };

    return 0;
}
