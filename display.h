/*
 * File: display.h
 * Author: dgrubb
 * Date: 07/17/2017
 *
 * Adds support for an SPI driven TFT screen.
 *
 * The screen I'm using for this project is a SpotPear 2.4inch TFT LCD from
 * as I have quite a few of them laying around from other projects:
 *
 * http://www.spotpear.com/index.php/spotpear-raspberry-pi-lcd/spotpear-raspberry-pi-2-4inch-lcd
 *
 * The screen is essentially a breakout board for the Z240IT008 v0.1 LCD screen:
 *
 * https://www.ezsolutionkr.com/tft-lcd-z240it008-v0-1
 *
 * which is, in turn, a clone of the ILI9341. Referential reading:
 *
 * 1) AdaFruit Arduino library for driving the ILI9341
 *      https://github.com/adafruit/Adafruit_ILI9341
 *
 * 2) Linux framebuffer driver for the ILI9341
 *      https://github.com/notro/fbtft/blob/master/fb_ili9341.c
 */

#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <stdint.h>
#include "sifive/devices/spi.h"

#define SPI_REG(x) SPI1_REG(x)

static const uint32_t SPI1_IOF_MASK =
    (1 << IOF_SPI1_SCK) |
    (1 << IOF_SPI1_MOSI) |
    (1 << IOF_SPI1_MISO);

void display_init();
void display_write(uint8_t data);

#endif /* _DISPLAY_H */
