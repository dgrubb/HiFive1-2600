/*
 * File: ili9341.c
 * Author: dgrubb
 * Date: 07/09/2018
 *
 * Drive display output of an ILI9341 SPI screen.
 */

#include "platform_util.h"
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
    delay_10ms(12); // Delay for 120ms
    ili9341_write_command(ILI9341_DISPON);      // Display on

    // TODO: Remove after testing, set the screen to ensure we're up and talking
    delay_10ms(100);
    uint16_t screen_colour = ili9341_colour_565(0x3F, 0x89, 0xFF);
    ili9341_fill_screen(screen_colour);
}

int ili9341_write_command(uint8_t command)
{
    GPIO_REG(GPIO_OUTPUT_VAL)   &= ~SPI_DC;
    GPIO_REG(GPIO_OUTPUT_VAL)   &= ~SPI_CS;
    spi_write(command);
    GPIO_REG(GPIO_OUTPUT_VAL)   |= SPI_CS;
}

int ili9341_write_data(uint8_t data)
{
    GPIO_REG(GPIO_OUTPUT_VAL)   |= SPI_DC;
    GPIO_REG(GPIO_OUTPUT_VAL)   &= ~SPI_CS;
    spi_write(data);
    GPIO_REG(GPIO_OUTPUT_VAL)   |= SPI_CS;
}

int ili9341_draw_line(tia_pixel_t *line_data, int line_length)
{
    int i;
    for (i=0; i<line_length; i++) {
        /* TODO: format each pixel value to an ILI pizel write command */
    }
}

int ili9341_fill_screen(uint16_t colour)
{
    return ili9341_fill_rectangle(0, 0, ILI9341_TFTWIDTH, ILI9341_TFTHEIGHT, colour);
}

int ili9341_fill_rectangle(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t colour)
{
    ili9341_set_address_window(x, y, x+width-1, y+height-1);
    uint8_t hi = (colour >> 8), lo = colour;

    GPIO_REG(GPIO_OUTPUT_VAL)   |= SPI_DC;
    GPIO_REG(GPIO_OUTPUT_VAL)   &= ~SPI_CS;

    for (y=height; y>0; y--) {
        for (x=width; x>0; x--) {
            spi_write(hi);
        }
        spi_write(lo);
    }

    GPIO_REG(GPIO_OUTPUT_VAL)   |= SPI_CS;
}

int ili9341_set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    ili9341_write_command(ILI9341_CASET);
    ili9341_write_data(x0 >> 8);
    ili9341_write_data(x0 & 0xFF);
    ili9341_write_data(x1 >> 8);
    ili9341_write_data(x1 & 0xFF);

    ili9341_write_command(ILI9341_PASET);
    ili9341_write_data(y0 >> 8);
    ili9341_write_data(y0);
    ili9341_write_data(y1 >> 8);
    ili9341_write_data(y1);
}

uint16_t ili9341_colour_565(uint8_t r, uint8_t g, uint8_t b)
{
    return (((r & 0xF8) << 8) | (g & 0xFC) | (b >> 3));
}
