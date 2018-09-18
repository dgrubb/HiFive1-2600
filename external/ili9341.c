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

int ili9341_init()
{
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

    ili9341_write_command(ILI9341_PWCTR1);      /* Power control */
    ili9341_write_data(0x23);                   /* VRH[5:0] */

    ili9341_write_command(ILI9341_PWCTR2);      /* Power control */
    ili9341_write_data(0x10);                   /* SAP[2:0];BT[3:0] */

    ili9341_write_command(ILI9341_VMCTR1);      /* VCM control */
    ili9341_write_data(0x3e);
    ili9341_write_data(0x28);

    ili9341_write_command(ILI9341_VMCTR2);      /* VCM control2 */
    ili9341_write_data(0x86);                   /* -- */

    ili9341_write_command(ILI9341_MADCTL);      /* Memory Access Control */
    ili9341_write_data(0x48);

    ili9341_write_command(ILI9341_PIXFMT);
    ili9341_write_data(0x55);

    ili9341_write_command(ILI9341_FRMCTR1);
    ili9341_write_data(0x00);
    ili9341_write_data(0x18);

    ili9341_write_command(ILI9341_DFUNCTR);     /* Display Function Control */
    ili9341_write_data(0x08);
    ili9341_write_data(0x82);
    ili9341_write_data(0x27);

    ili9341_write_command(0xF2);                /* 3Gamma Function Disable */
    ili9341_write_data(0x00);

    ili9341_write_command(ILI9341_GAMMASET);    /* Gamma curve selected */
    ili9341_write_data(0x01);

    ili9341_write_command(ILI9341_GMCTRP1);     /* Set Gamma */
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

    ili9341_write_command(ILI9341_GMCTRN1);    /* Set Gamma */
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

    /* Make origin (0, 0) the top-left of screen, i.e., close to the RESET button corner*/
    ili9341_write_command(ILI9341_MADCTL);
    ili9341_write_data(MADCTL_MV | MADCTL_BGR);

    ili9341_write_command(ILI9341_SLPOUT);      /* Exit Sleep */
    delay_10ms(12);                             /* Delay for 120ms, let everything settle */
    ili9341_write_command(ILI9341_DISPON);      /* Display on */

    /* Finish intialisation by filling the screen with bright blue. Provides a 
     * visual indication that everything up until now is functioning.
     */
    delay_10ms(10);
    ili9341_clear_screen();
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

int ili9341_draw_line(tia_pixel_t *line_data, int y, int line_length)
{
    int i;
    for (i=0; i<line_length; i++) {
        ili9341_fill_rectangle(
            ili9341_scale_horizontal(i),
            ili9341_scale_vertical(y),
            ili9341_scale_horizontal(1),
            ili9341_scale_vertical(1),
            ili9341_colour_565(line_data[i].R, line_data[i].G, line_data[i].B)
        );
    }
}

int ili9341_clear_screen()
{
    return ili9341_fill_screen(0x0000);
}

int ili9341_fill_screen(uint16_t colour)
{
    return ili9341_fill_rectangle(0, 0, ILI9341_TFTWIDTH, ILI9341_TFTHEIGHT, colour);
}

int ili9341_fill_rectangle(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t colour)
{
    uint8_t hi = (colour >> 8), lo = colour;
    if((x + width - 1) >= ILI9341_TFTWIDTH) width = ILI9341_TFTWIDTH - x;
    if((y + height - 1) >= ILI9341_TFTHEIGHT) height = ILI9341_TFTHEIGHT - y;
    ili9341_set_address_window(x, y, x+width-1, y+height-1);

    GPIO_REG(GPIO_OUTPUT_VAL)   |= SPI_DC;
    GPIO_REG(GPIO_OUTPUT_VAL)   &= ~SPI_CS;

    for (y=height; y>0; y--) {
        for (x=width; x>0; x--) {
            spi_write(hi);
            spi_write(lo);
        }
    }

    GPIO_REG(GPIO_OUTPUT_VAL)   |= SPI_CS;
}

uint16_t ili9341_scale_to_range(uint16_t input_val, uint16_t target_min_range,
    uint16_t target_max_range, uint16_t input_min_range, uint16_t input_max_range)
{
    uint16_t scaled_value = (target_max_range - target_min_range) * 
        (input_val - input_min_range)/(input_max_range-input_min_range) +
        target_min_range;
    return scaled_value;
}

uint16_t ili9341_scale_horizontal(uint16_t unscaled_length)
{
    return ili9341_scale_to_range(
        unscaled_length,
        0,
        ILI9341_TFTWIDTH,
        0,
        ATARI_RESOLUTION_WIDTH
    );
}

uint16_t ili9341_scale_vertical(uint16_t unscaled_length)
{
    return ili9341_scale_to_range(
        unscaled_length,
        0,
        ILI9341_TFTHEIGHT,
        0,
        ATARI_RESOLUTION_HEIGHT
    );
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

    ili9341_write_command(ILI9341_RAMWR);
}

uint16_t ili9341_colour_565(uint8_t r, uint8_t g, uint8_t b)
{
    return (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3));
}
