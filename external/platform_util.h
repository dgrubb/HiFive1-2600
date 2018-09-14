/*
 * File: platform_util.h
 * Author: dgrubb
 * Date: 09/07/2018
 *
 * Provides 
 */

#ifndef _PLATFORM_UTIL_H
#define _PLATFORM_UTIL_H

/* Standard includes */
#include <stdint.h>

/* HiFive1/FE310 includes */
#include "encoding.h"
#include "platform.h"
#include "plic/plic_driver.h"

/* Calculation assumes a core frequency of ~262MHz and pwmscale of 1.
 * Integer value is a result of:
 *
 * (Clock source / divisor) / FREQUENCY
 *
 * Eg., 3.58MHz TIA input clock::
 *
 * (262MHz / 2^1) / 3580000 = 36 (0x24 in hex)
 *
 * #define PWM_FREQ            0x24
 * #define PWM_FREQ_DIV        0x0C
 * #define PWM_SCALE           0x01
*/

/* The Atari 2600 had 288 discrete colour clocks per line at a frequency of
 * 3.58MHz. This timeout provides a period of 0.080446927ms, that is:
 *
 * (1/3.58MHz) * 288
 */
#define PWM_FREQ            0x104C
#define PWM_FREQ_DIV        0x0826
#define PWM_SCALE           0x0002

/* Provides a timer which times out after 10ms, used for creating arbitary
 * delays with lengths of multiples of 10ms. See delay_10ms().
 */
#define PWM_TIMER_FREQ      0xFFDC
#define PWM_TIMER_FREQ_DIV  0x7FEE /* PWM_TIMER_FREQ/2 for 50% duty */
#define PWM_TIMER_SCALE     0x0002

/* I/O masks */
#define BLUE_LED_MASK     0x1 << BLUE_LED_OFFSET
#define GREEN_LED_MASK    0x1 << GREEN_LED_OFFSET

#define ATARI_RESOLUTION_WIDTH  160
#define ATARI_RESOLUTION_HEIGHT 192

void init_clock();
void init_timer();
void init_GPIO();
void delay_10ms(uint32_t multiplier);
void enable_interrupts();

#endif /* _PLATFORM_UTIL_H */
