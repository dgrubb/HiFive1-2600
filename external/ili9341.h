/*
 * File: ili9341.h
 * Author: dgrubb
 * Date: 07/09/2018
 *
 * Drive display output of an ILI9341 SPI screen.
 */


#ifndef _ILI9341_H
#define _ILI9341_H

#include "atari/Atari-TIA.h"

int ili9341_init();
int ili9341_write_command();
int ili9341_draw_line(tia_pixel *line_data, int line_length);

#endif /* _ILI9341_H */
