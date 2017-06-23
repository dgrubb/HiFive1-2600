/* 
 * File: Atari-TIA.h
 * Author: dgrubb
 * Date: 06/23/17
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

#ifndef _TIA_H
#define _TIA_H

#include <stdint.h>

/* Provides a memory mapping used by the TIA */
typedef struct {

} atari_tia;

extern atari_tia tia;

#endif /* _TIA_H */
