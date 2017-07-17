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
    /* Sets up the local hardware peripheral */
    display_init_spi();
    /* Sets up the external display hardware */
    display_init_sequence();
}

void display_init_sequence()
{
    display_write(0xEF); /* CMD */
    display_write(0x03);
    display_write(0x80);
    display_write(0x02);

    display_write(0xCF); /* CMD */
    display_write(0x00);
    display_write(0xC1);
    display_write(0x30);

    display_write(0xED); /* CMD */
    display_write(0x64);
    display_write(0x03);
    display_write(0x12);
    display_write(0x81);

    display_write(0xE8); /* CMD */
    display_write(0x85);
    display_write(0x00);
    display_write(0x78);

    display_write(0xCB); /* CMD */
    display_write(0x39);
    display_write(0x2C);
    display_write(0x00);
    display_write(0x34);
    display_write(0x02);

    display_write(0xF7); /* CMD */
    display_write(0x20);

    display_write(0xEA); /* CMD */
    display_write(0x00);
    display_write(0x00);

    display_write(ILI9341_PWCTR1);    //Power control
    display_write(0x23);   //VRH[5:0]

    display_write(ILI9341_PWCTR2);    //Power control
    display_write(0x10);   //SAP[2:0];BT[3:0]

    display_write(ILI9341_VMCTR1);    //VCM control
    display_write(0x3e);
    display_write(0x28);

    display_write(ILI9341_VMCTR2);    //VCM control2
    display_write(0x86);  //--

    display_write(ILI9341_MADCTL);    // Memory Access Control
    display_write(0x48);

    display_write(ILI9341_VSCRSADD); // Vertical scroll
    display_write(0x00);                 // Zero
    display_write(0x00);                 // Zero

    display_write(ILI9341_PIXFMT);
    display_write(0x55);

    display_write(ILI9341_FRMCTR1);
    display_write(0x00);
    display_write(0x18);

    display_write(ILI9341_DFUNCTR);    // Display Function Control
    display_write(0x08);
    display_write(0x82);
    display_write(0x27);

    display_write(0xF2);    // 3Gamma Function Disable
    display_write(0x00);

    display_write(ILI9341_GAMMASET);    //Gamma curve selected
    display_write(0x01);

    display_write(ILI9341_GMCTRP1);    //Set Gamma
    display_write(0x0F);
    display_write(0x31);
    display_write(0x2B);
    display_write(0x0C);
    display_write(0x0E);
    display_write(0x08);
    display_write(0x4E);
    display_write(0xF1);
    display_write(0x37);
    display_write(0x07);
    display_write(0x10);
    display_write(0x03);
    display_write(0x0E);
    display_write(0x09);
    display_write(0x00);

    display_write(ILI9341_GMCTRN1);    //Set Gamma
    display_write(0x00);
    display_write(0x0E);
    display_write(0x14);
    display_write(0x03);
    display_write(0x11);
    display_write(0x07);
    display_write(0x31);
    display_write(0xC1);
    display_write(0x48);
    display_write(0x08);
    display_write(0x0F);
    display_write(0x0C);
    display_write(0x31);
    display_write(0x36);
    display_write(0x0F);

    display_write(ILI9341_SLPOUT);    //Exit Sleep
    // delay(120);
    display_write(ILI9341_DISPON);    //Display on
    // delay(120);
}

void display_init_spi()
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
     * 262MHz / 2(2+1) = 43.67MHz
     */
    SPI_REG(SPI_REG_SCKDIV) = 0x02;
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
