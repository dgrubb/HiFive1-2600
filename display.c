/*
 * File: display.c
 * Author: dgrubb
 * Date: 07/17/2017
 *
 * Adds support for an SPI driven TFT screen.
 */

#include "display.h"

void display_init()
{
    /* Full documentation for the FE310 SPI module is at:
     * https://www.sifive.com/documentation/freedom-soc/freedom-e300-platform-reference-manual
     * Chapter 13
     */

    /* Select and enable SPI1 device pins */
    GPIO_REG(GPIO_IOF_SEL) &= ~SPI1_IOF_MASK;
    GPIO_REG(GPIO_IOF_EN)  |=  SPI1_IOF_MASK;

    /* Set data mode
     * Bit 0: pha - Inactive state of SCK is logical 0
     * Bit 1: pol - Data is sampled on the leading edge of SCK
     */
    SPI_REG(SPI_REG_SCKMODE) = 0x0;

    /* Set frame format register
     * Bit 0-1: proto - SPI protocol used. SPI_PROTO_S = single channel
     * Bit 2: endian - Endianess of frame. SPI_ENDIAN_MSB = transmit MSB first
     * Bit 3: dir - Allows RX during dual and quad modes
     * Bit 16-19: len - length of frame
     */
    SPI_REG(SPI_REG_FMT) = 0;
    SPI_REG(SPI_REG_FMT) =
        SPI_FMT_PROTO(SPI_PROTO_S)     |
        SPI_FMT_ENDIAN(SPI_ENDIAN_MSB) |
        SPI_FMT_DIR(SPI_DIR_RX)        |
        SPI_FMT_LEN(8); // 8 bit long packets

    /* Set CS mode auto
     * SPI_CSMODE_AUTO - Assert/de-assert CS at beginning and end of each frame
     */
    SPI_REG(SPI_REG_CSMODE) = SPI_CSMODE_AUTO;

    /* Clock divider
     * Original clock is coreclk (the main CPU clock) where the resulting SPI
     * clock speed is determined by:
     *
     * sck = coreclk / 2(divider + 1)
     *
     * Defaults to 0x003. E.g.,:
     *
     * 262MHz / 2(3+1) = 32.75MHz
     */
    SPI_REG(SPI_REG_SCKDIV) = 0x03;
}

void display_write(uint8_t data)
{
    /* Set SS low during trasmission */
    GPIO_REG(GPIO_OUTPUT_VAL) &= ~(0x1 << PIN_10_OFFSET);

    /* Flush transmit buffer and send data */
    while (SPI_REG(SPI_REG_TXFIFO) & SPI_TXFIFO_FULL);
    SPI_REG(SPI_REG_TXFIFO) = data;

    /* Set SS to high to release device */
    GPIO_REG(GPIO_OUTPUT_VAL) |= (0x1 << PIN_10_OFFSET);
}
