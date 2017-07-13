/*
 * File: clocks.h
 * Author: dgrubb
 * Date: 07/13/2017
 *
 * Implements system timers and clocks.
 */

/* HiFive1/FE310 includes */
#include "platform.h"
#include "encoding.h"
#include "plic/plic_driver.h"

/* Atari includes */
#include "mos6507.h"
#include "clocks.h"

/* Calculation assumes a core frequency of ~262MHz and pwmscale of 1.
 * Integer value is a result of:
 *
 * (Clock source / divisor) / FREQUENCY
 *
 * Eg., 3.58MHz TIA input clock::
 *
 * (262MHz / 2^1) / 3580000 = 36
 */
#define PWM_SCALE 0x1
#define PWM_FREQ  0x24

// Instance data for the PLIC.
static plic_instance_t g_plic;

void handle_m_time_interrupt()
{
}

void handle_m_ext_interrupt()
{
    plic_source int_num = PLIC_claim_interrupt(&g_plic);
    switch (int_num) {
        case 0:
            break;
        case INT_PWM1_BASE:
            PWM1_REG(PWM_CFG) &= ~PWM_CFG_CMP0IP;
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

    /* Re-enable timers */
    set_csr(mie, MIP_MTIP);
    set_csr(mstatus, MSTATUS_MIE);
}

