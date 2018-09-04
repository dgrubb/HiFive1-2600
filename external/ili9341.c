/*
 * File: ili9341.c
 * Author: dgrubb
 * Date: 07/09/2018
 *
 * Drive display output of an ILI9341 SPI screen.
 */

#include "ili9341.h"
#include "spi.h"

int ili9341_reset()
{

    /*
    digitalWrite(_rst, HIGH);
    delay(5);
    digitalWrite(_rst, LOW);
    delay(20);
    digitalWrite(_rst, HIGH);
    delay(150);
    */
}

int ili9341_init()
{
    ili9341_reset();

    ili9341_write_command(0xEF);
    ili9341_write_data(0x03);
    ili9341_write_data(0x80);
    ili9341_write_data(0x02);

    ili9341_write_command(0xCF);
    ili9341_write_data(0x00);
    ili9341_write_data(0xC1);
    ili9341_write_data(0x30);

    ili9341_write_command(0xED);
    ili9341_write_data(0x64);
    ili9341_write_data(0x03);
    ili9341_write_data(0x12);
    ili9341_write_data(0x81);

    ili9341_write_command(0xE8);
    ili9341_write_data(0x85);
    ili9341_write_data(0x00);
    ili9341_write_data(0x78);

    ili9341_write_command(0xCB);
    ili9341_write_data(0x39);
    ili9341_write_data(0x2C);
    ili9341_write_data(0x00);
    ili9341_write_data(0x34);
    ili9341_write_data(0x02);

    ili9341_write_command(0xF7);
    ili9341_write_data(0x20);

    ili9341_write_command(0xEA);
    ili9341_write_data(0x00);
    ili9341_write_data(0x00);

    ili9341_write_command(ILI9341_PWCTR1);      // Power control
    ili9341_write_data(0x23);                   // VRH[5:0]

    ili9341_write_command(ILI9341_PWCTR2);      // Power control
    ili9341_write_data(0x10);                   // SAP[2:0];BT[3:0]

    ili9341_write_command(ILI9341_VMCTR1);      // VCM control
    ili9341_write_data(0x3e);
    ili9341_write_data(0x28);

    ili9341_write_command(ILI9341_VMCTR2);      // VCM control2
    ili9341_write_data(0x86);                   // --

    ili9341_write_command(ILI9341_MADCTL);      // Memory Access Control
    ili9341_write_data(0x48);

    ili9341_write_command(ILI9341_PIXFMT);
    ili9341_write_data(0x55);

    ili9341_write_command(ILI9341_FRMCTR1);
    ili9341_write_data(0x00);
    ili9341_write_data(0x18);

    ili9341_write_command(ILI9341_DFUNCTR);     // Display Function Control
    ili9341_write_data(0x08);
    ili9341_write_data(0x82);
    ili9341_write_data(0x27);

    ili9341_write_command(0xF2);                // 3Gamma Function Disable
    ili9341_write_data(0x00);

    ili9341_write_command(ILI9341_GAMMASET);    // Gamma curve selected
    ili9341_write_data(0x01);

    ili9341_write_command(ILI9341_GMCTRP1);     // Set Gamma
    ili9341_write_data(0x0F);
    ili9341_write_data(0x31);
    ili9341_write_data(0x2B);
    ili9341_write_data(0x0C);
    ili9341_write_data(0x0E);
    ili9341_write_data(0x08);
    ili9341_write_data(0x4E);
    ili9341_write_data(0xF1);
    ili9341_write_data(0x37);
    ili9341_write_data(0x07);
    ili9341_write_data(0x10);
    ili9341_write_data(0x03);
    ili9341_write_data(0x0E);
    ili9341_write_data(0x09);
    ili9341_write_data(0x00);

    ili9341_write_command(ILI9341_GMCTRN1);    // Set Gamma
    ili9341_write_data(0x00);
    ili9341_write_data(0x0E);
    ili9341_write_data(0x14);
    ili9341_write_data(0x03);
    ili9341_write_data(0x11);
    ili9341_write_data(0x07);
    ili9341_write_data(0x31);
    ili9341_write_data(0xC1);
    ili9341_write_data(0x48);
    ili9341_write_data(0x08);
    ili9341_write_data(0x0F);
    ili9341_write_data(0x0C);
    ili9341_write_data(0x31);
    ili9341_write_data(0x36);
    ili9341_write_data(0x0F);

    ili9341_write_command(ILI9341_SLPOUT);      // Exit Sleep
    // TODO: delay for 120ms here?
    ili9341_write_command(ILI9341_DISPON);      // Display on
}

int ili9341_write_command(uint8_t command)
{
    spi_write(command);
}

int ili9341_write_data(uint8_t data)
{
    spi_write(data);
}

int ili9341_draw_line(tia_pixel_t *line_data, int line_length)
{
    int i;
    for (i=0; i<line_length; i++) {
        /* TODO: format each pixel value to an ILI pizel write command */
    }
}

