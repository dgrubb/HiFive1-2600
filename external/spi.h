/*
 * File: spi.h
 * Author: dgrubb
 * Date: 08/30/2017
 *
 * Handle I/O with external peripherals on the SPI interface.
 */

#ifndef _SPI_H
#define _SPI_H

/* Standard includes */
#include <stdint.h>

/* HiFive1/FE310 includes */
#include "sifive/devices/spi.h"
#include "platform.h"

/* Hardcode to use device SPI1 */
#define SPI_REG(x) SPI1_REG(x)
#define RTC_FREQUENCY 32768

#define SPI_READ  0x01
#define SPI_WRITE 0x00

static const uint32_t SPI1_IOF_MASK =
    (1 << IOF_SPI1_SS0)  |
    (1 << IOF_SPI1_SCK)  |
    (1 << IOF_SPI1_MOSI) |
    (1 << IOF_SPI1_MISO);

void init_SPI();
void spi_write(uint8_t data);

#endif /* _SPI_H */

