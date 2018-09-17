/*
 * File: platform_util.c
 * Author: dgrubb
 * Date: 09/07/2018
 *
 * Provides general utility and helper functions predicated on the platform
 * hardware.
 */

#include "platform_util.h"
#include "ili9341.h"
#include "atari/Atari-TIA.h"
#include "spi.h"

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

void init_timer()
{
    PWM2_REG(PWM_CFG) = 0;

    PWM2_REG(PWM_CFG) =
        (PWM_CFG_DEGLITCH)   |
        (PWM_CFG_CMP2CENTER) |
        (PWM_CFG_CMP3CENTER) |
        (PWM_CFG_ZEROCMP)    |
        (PWM_SCALE);
    PWM2_REG(PWM_CMP0) = PWM_TIMER_FREQ;
    PWM2_REG(PWM_CMP3) = PWM_TIMER_FREQ_DIV; /* Output a 50% duty cycle */

    PWM2_REG(PWM_COUNT) = 0;
}

void init_GPIO()
{
    /* Debug output indicator */
    GPIO_REG(GPIO_OUTPUT_EN)   |=   BLUE_LED_MASK;
    GPIO_REG(GPIO_OUTPUT_VAL)  &=  ~BLUE_LED_MASK;

    /* Clock source ouput (PWM) */
    GPIO_REG(GPIO_IOF_SEL)     |=   GREEN_LED_MASK;
    GPIO_REG(GPIO_IOF_EN)      |=   GREEN_LED_MASK;

    /* The Adafruit ILI9341 TFT screen has a few extra non-standard control pins
     * (such as reset and data/command selector). We'll be controlling these
     * through manual GPIO selection.
     */
    GPIO_REG(GPIO_OUTPUT_XOR)  &=   ~(SPI_DC | SPI_CS);
    GPIO_REG(GPIO_IOF_EN)      &=   ~(SPI_DC | SPI_CS);
    GPIO_REG(GPIO_INPUT_EN)    &=   ~(SPI_DC | SPI_CS);
    GPIO_REG(GPIO_OUTPUT_EN)   |=    (SPI_DC | SPI_CS);
    GPIO_REG(GPIO_PULLUP_EN)   &=   ~(SPI_DC | SPI_CS);
    GPIO_REG(GPIO_OUTPUT_VAL)  &=   ~(SPI_DC | SPI_CS);

}

/******************************************************************************
 * Hardware-based utilities
 *****************************************************************************/

void delay_10ms(uint32_t multiplier)
{
    int i;
    for (i=0; i<multiplier; i++) {
        PWM2_REG(PWM_CFG) |= PWM_CFG_ONESHOT;
        while (PWM2_REG(PWM_COUNT)) {}
    }
}
void enable_interrupts()
{
    set_csr(mie, MIP_MEIP);
    set_csr(mie, MIP_MTIP);
    set_csr(mstatus, MSTATUS_MIE);
}

#ifdef COLOUR_TEST
void colour_test()
{
    int line_count=0;
    while(1) {
        for (line_count=0; line_count<ATARI_RESOLUTION_HEIGHT; line_count++) {
            ili9341_draw_line(tia_test_line, line_count, ATARI_RESOLUTION_WIDTH);
        }
    }

}
#endif /* COLOUR_TEST */
