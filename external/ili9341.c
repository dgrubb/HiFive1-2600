/*
 * File: ili9341.c
 * Author: dgrubb
 * Date: 07/09/2018
 *
 * Drive display output of an ILI9341 SPI screen.
 */

#include "ili9341.h"
#include "spi.h"

int ili9341_init()
{
}

int ili9341_write_command()
{
}

int ili9341_write_data()
{
}

int ili9341_draw_line(tia_pixel *line_data, int line_length)
{
    int i;
    for (i=0; i<line_length; i++) {
        /* TODO: format each pixel value to an ILI pizel write command */
    }
}

