/*
 * File: Atari-TIA.c
 * Author: dgrubb
 * Date: 06/23/2017
 *
 * Implements the Atari 2600's custom Television Interface Adapter chip. This
 * provides audio and graphics drawing via analog beam chasing methods (as
 * opposed to a stored framebuffer). The TIA also provides features for
 * providing simple game objects such as playfield, players and missiles.
 *
 * References:
 *
 * Stella Programmer's Guide - http://atarihq.com/danb/files/stella.pdf
 * Chip pinout - http://atarihq.com/danb/tia.shtml
 */

#include "Atari-TIA.h"

/* Singleton representation of a TIA chip */
atari_tia tia;

/* Resets the TIA instance to default conditions with no state set.
 */
void TIA_init()
{
    int i = 0;
    tia.address_bus = 0;
    tia.data_bus = 0;
    for (i=0; i<WRITABLE_REG_LEN; i++) {
        tia.write_regs[i] = 0;
    }
    for (i=0; i<READABLE_REG_LEN; i++) {
        tia.read_regs[i] = 0;
    }
    tia.databus_direction = WRITE;
    tia.chip_select = 0;
}

