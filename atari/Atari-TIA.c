/*
 * File: Atari-TIA.c
 * Author: dgrubb
 * Date: 09/18/2017
 *
 * Implements the TIA memory map.
 */

// TODO remove
#include <stdio.h>

#include "Atari-TIA.h"
#include "external/ili9341.h"
#include "external/platform_util.h"

atari_tia tia;

tia_pixel_t tia_line_buffer[];
/* See page 40 of docs/Stella Programmer's Guide.pdf */
uint8_t tia_player_size_map[] = {
    0x80, /* 0: One copy */
    0xA0, /* 1: Two copies - close */
    0x90, /* 2: Two copies - medium */
    0xA8, /* 3: Three copies - close */
    0x81, /* 4: Two copies - wide */
    0xC0, /* 5: Double size player */
    0x89, /* 6: Three copies medium */
    0xF0  /* 7: Quad-sized player */
};

/* Usage note:
 *
 * This table is mapped to allow for lookup from a value in one of the colour
 * registers. However, the colour reigsters don't use the least significant
 * bit so the table locations assume the value has been shifted by one bit
 * for easier alignment. See page 43 of docs/Stella Programmer's guide.pdf
 *
 * N.B: only the NTSC colour pallete is supported.
 */
tia_pixel_t tia_colour_map[] = {
                          /* TIA:  >>1:  Index (dec): */
    { 0x00, 0x00, 0x00 }, /* 0x00, 0x00, 0 */
    { 0x1A, 0x1A, 0x1A }, /* 0x02, 0x01, 1 */
    { 0x39, 0x39, 0x39 }, /* 0x04, 0x02, 2 */
    { 0x58, 0x58, 0x58 }, /* 0x06, 0x03, 3 */
    { 0x7E, 0x7E, 0x7E }, /* 0x08, 0x04, 4 */
    { 0xA2, 0xA2, 0xA2 }, /* 0x0A, 0x05, 5 */
    { 0xC7, 0xC7, 0xC7 }, /* 0x0C, 0x06, 6 */
    { 0xED, 0xED, 0xED }, /* 0x0E, 0x07, 7 */
    { 0x19, 0x02, 0x00 }, /* 0x10, 0x08, 8 */
    { 0x3A, 0x1F, 0x00 }, /* 0x12, 0x09, 9 */
    { 0x5D, 0x41, 0x00 }, /* 0x14, 0x0A, 10 */
    { 0x82, 0x64, 0x00 }, /* 0x16, 0x0B, 11 */
    { 0xA7, 0x88, 0x00 }, /* 0x18, 0x0C, 12 */
    { 0xCC, 0xAD, 0x00 }, /* 0x1A, 0x0D, 13 */
    { 0xF2, 0xD2, 0x19 }, /* 0x1C, 0x0E, 14 */
    { 0xFE, 0xFA, 0x40 }, /* 0x1E, 0x0F, 15 */
    { 0x37, 0x00, 0x00 }, /* 0x20, 0x10, 16 */
    { 0x5E, 0x08, 0x00 }, /* 0x22, 0x11, 17 */
    { 0x83, 0x27, 0x00 }, /* 0x24, 0x12, 18 */
    { 0xA9, 0x49, 0x00 }, /* 0x26, 0x13, 19 */
    { 0xCF, 0x6C, 0x00 }, /* 0x28, 0x14, 20 */
    { 0xF5, 0x8F, 0x17 }, /* 0x2A, 0x15, 21 */
    { 0xFE, 0xB4, 0x38 }, /* 0x2C, 0x16, 22 */
    { 0xFE, 0xDF, 0x6F }, /* 0x2E, 0x17, 23 */
    { 0x47, 0x00, 0x00 }, /* 0x30, 0x18, 24 */
    { 0x73, 0x00, 0x00 }, /* 0x32, 0x19, 25 */
    { 0x98, 0x13, 0x00 }, /* 0x34, 0x1A, 26 */
    { 0xBE, 0x32, 0x16 }, /* 0x36, 0x1B, 27 */
    { 0xE4, 0x53, 0x35 }, /* 0x38, 0x1C, 28 */
    { 0xFE, 0x76, 0x57 }, /* 0x3A, 0x1D, 29 */
    { 0xFE, 0x9C, 0x81 }, /* 0x3C, 0x1E, 30 */
    { 0xFE, 0xC6, 0xBB }, /* 0x3E, 0x1F, 31 */
    { 0x44, 0x00, 0x08 }, /* 0x40, 0x20, 32 */
    { 0x6F, 0x00, 0x1F }, /* 0x42, 0x21, 33 */
    { 0x96, 0x06, 0x40 }, /* 0x44, 0x22, 34 */
    { 0xBB, 0x24, 0x62 }, /* 0x46, 0x23, 35 */
    { 0xE1, 0x45, 0x85 }, /* 0x48, 0x24, 36 */
    { 0xFE, 0x67, 0xAA }, /* 0x4A, 0x25, 37 */
    { 0xFE, 0x8C, 0xD6 }, /* 0x4C, 0x26, 38 */
    { 0xFE, 0xB7, 0xF6 }, /* 0x4E, 0x27, 39 */
    { 0x2D, 0x00, 0x4A }, /* 0x50, 0x28, 40 */
    { 0x57, 0x00, 0x67 }, /* 0x52, 0x29, 41 */
    { 0x7D, 0x05, 0x8C }, /* 0x54, 0x2A, 42 */
    { 0xA1, 0x22, 0xB1 }, /* 0x56, 0x2B, 43 */
    { 0xC7, 0x43, 0xD7 }, /* 0x58, 0x2C, 44 */
    { 0xED, 0x65, 0xFE }, /* 0x5A, 0x2D, 45 */
    { 0xFE, 0x8A, 0xF6 }, /* 0x5C, 0x2E, 46 */
    { 0xFE, 0xB5, 0xF7 }, /* 0x5E, 0x2F, 47 */
    { 0x0D, 0x00, 0x82 }, /* 0x60, 0x30, 48 */
    { 0x33, 0x00, 0xA2 }, /* 0x62, 0x31, 49 */
    { 0x55, 0x0F, 0xC9 }, /* 0x64, 0x32, 50 */
    { 0x78, 0x2D, 0xF0 }, /* 0x66, 0x33, 51 */
    { 0x9C, 0x4E, 0xFE }, /* 0x68, 0x34, 52 */
    { 0xC3, 0x72, 0xFE }, /* 0x6A, 0x35, 53 */
    { 0xEB, 0x98, 0xFE }, /* 0x6C, 0x36, 54 */
    { 0xFE, 0xC0, 0xF9 }, /* 0x6E, 0x37, 55 */
    { 0x00, 0x00, 0x91 }, /* 0x70, 0x38, 56 */
    { 0x0A, 0x05, 0xBD }, /* 0x72, 0x39, 57 */
    { 0x28, 0x22, 0xE4 }, /* 0x74, 0x3A, 58 */
    { 0x48, 0x42, 0xFE }, /* 0x76, 0x3B, 59 */
    { 0x6B, 0x64, 0xFE }, /* 0x78, 0x3C, 50 */
    { 0x90, 0x8A, 0xFE }, /* 0x7A, 0x3D, 51 */
    { 0xB7, 0xB0, 0xFE }, /* 0x7C, 0x3E, 52 */
    { 0xDF, 0xD8, 0xFE }, /* 0x7E, 0x3F, 53 */
    { 0x00, 0x00, 0x72 }, /* 0x80, 0x40, 54 */
    { 0x00, 0x1C, 0xAB }, /* 0x82, 0x41, 55 */
    { 0x03, 0x3C, 0xD6 }, /* 0x84, 0x42, 56 */
    { 0x20, 0x5E, 0xFD }, /* 0x86, 0x43, 57 */
    { 0x40, 0x81, 0xFE }, /* 0x88, 0x44, 58 */
    { 0x64, 0xA6, 0xFE }, /* 0x8A, 0x45, 59 */
    { 0x89, 0xCE, 0xFE }, /* 0x8C, 0x46, 60 */
    { 0xB0, 0xF6, 0xFE }, /* 0x8E, 0x47, 61 */
    { 0x00, 0x10, 0x3A }, /* 0x90, 0x48, 62 */
    { 0x00, 0x31, 0x6E }, /* 0x92, 0x49, 63 */
    { 0x00, 0x55, 0xA2 }, /* 0x94, 0x4A, 64 */
    { 0x05, 0x79, 0xC8 }, /* 0x96, 0x4B, 65 */
    { 0x23, 0x9D, 0xEE }, /* 0x98, 0x4C, 66 */
    { 0x44, 0xC2, 0xFE }, /* 0x9A, 0x4D, 67 */
    { 0x68, 0xE9, 0xFE }, /* 0x9C, 0x4E, 68 */
    { 0x8F, 0xFE, 0xFE }, /* 0x9E, 0x4F, 69 */
    { 0x00, 0x1F, 0x02 }, /* 0xA0, 0x50, 70 */
    { 0x00, 0x43, 0x26 }, /* 0xA2, 0x51, 71 */
    { 0x00, 0x69, 0x57 }, /* 0xA4, 0x52, 72 */
    { 0x00, 0x8D, 0x7A }, /* 0xA6, 0x53, 73 */
    { 0x1B, 0xB1, 0x9E }, /* 0xA8, 0x54, 74 */
    { 0x3B, 0xD7, 0xC3 }, /* 0xAA, 0x55, 75 */
    { 0x5D, 0xFE, 0xE9 }, /* 0xAC, 0x56, 76 */
    { 0x86, 0xFE, 0xFE }, /* 0xAE, 0x57, 77 */
    { 0x00, 0x24, 0x03 }, /* 0xB0, 0x58, 78 */
    { 0x00, 0x4A, 0x05 }, /* 0xB2, 0x59, 79 */
    { 0x00, 0x70, 0x0C }, /* 0xB4, 0x5A, 80 */
    { 0x09, 0x95, 0x2B }, /* 0xB6, 0x5B, 81 */
    { 0x28, 0xBA, 0x4C }, /* 0xB8, 0x5C, 82 */
    { 0x49, 0xE0, 0x6E }, /* 0xBA, 0x5D, 83 */
    { 0x6C, 0xFE, 0x92 }, /* 0xBC, 0x5E, 84 */
    { 0x97, 0xFE, 0xB5 }, /* 0xBE, 0x5F, 85 */
    { 0x00, 0x21, 0x02 }, /* 0xC0, 0x60, 86 */
    { 0x00, 0x46, 0x04 }, /* 0xC2, 0x61, 87 */
    { 0x08, 0x6B, 0x00 }, /* 0xC4, 0x62, 88 */
    { 0x28, 0x90, 0x00 }, /* 0xC6, 0x63, 89 */
    { 0x49, 0xB5, 0x09 }, /* 0xC8, 0x64, 90 */
    { 0x6B, 0xDB, 0x28 }, /* 0xCA, 0x65, 91 */
    { 0x8F, 0xFE, 0x49 }, /* 0xCC, 0x66, 92 */
    { 0xBB, 0xFE, 0x69 }, /* 0xCE, 0x67, 93 */
    { 0x00, 0x15, 0x01 }, /* 0xD0, 0x68, 94 */
    { 0x10, 0x36, 0x00 }, /* 0xD2, 0x69, 95 */
    { 0x30, 0x59, 0x00 }, /* 0xD4, 0x6A, 96 */
    { 0x53, 0x7E, 0x00 }, /* 0xD6, 0x6B, 97 */
    { 0x76, 0xA3, 0x00 }, /* 0xD8, 0x6C, 98 */
    { 0x9A, 0xC8, 0x00 }, /* 0xDA, 0x6D, 99 */
    { 0xBF, 0xEE, 0x1E }, /* 0xDC, 0x6E, 100 */
    { 0xE8, 0xFE, 0x3E }, /* 0xDE, 0x6F, 101 */
    { 0x1A, 0x02, 0x00 }, /* 0xE0, 0x70, 102 */
    { 0x3B, 0x1F, 0x00 }, /* 0xE2, 0x71, 103 */
    { 0x5E, 0x41, 0x00 }, /* 0xE4, 0x72, 104 */
    { 0x83, 0x64, 0x00 }, /* 0xE6, 0x73, 105 */
    { 0xA8, 0x88, 0x00 }, /* 0xE8, 0x74, 106 */
    { 0xCE, 0xAD, 0x00 }, /* 0xEA, 0x75, 107 */
    { 0xF4, 0xD2, 0x18 }, /* 0xEC, 0x76, 108 */
    { 0xFE, 0xFA, 0x40 }, /* 0xEE, 0x77, 109 */
    { 0x38, 0x00, 0x00 }, /* 0xF0, 0x78, 110 */
    { 0x5F, 0x08, 0x00 }, /* 0xF2, 0x79, 111 */
    { 0x84, 0x27, 0x00 }, /* 0xF4, 0x7A, 112 */
    { 0xAA, 0x49, 0x00 }, /* 0xF6, 0x7B, 113 */
    { 0xD0, 0x6B, 0x00 }, /* 0xF8, 0x7C, 114 */
    { 0xF6, 0x8F, 0x18 }, /* 0xFA, 0x7D, 115 */
    { 0xFE, 0xB4, 0x39 }, /* 0xFC, 0x7E, 116 */
    { 0xFE, 0xDF, 0x70 }  /* 0xFE, 0x7F, 117 */
};

#ifdef COLOUR_TEST
tia_pixel_t tia_test_line[] = {
    { 0x00, 0x00, 0x00 }, /* 0x00, 0x00, 0 */
    { 0x1A, 0x1A, 0x1A }, /* 0x02, 0x01, 1 */
    { 0x39, 0x39, 0x39 }, /* 0x04, 0x02, 2 */
    { 0x58, 0x58, 0x58 }, /* 0x06, 0x03, 3 */
    { 0x7E, 0x7E, 0x7E }, /* 0x08, 0x04, 4 */
    { 0xA2, 0xA2, 0xA2 }, /* 0x0A, 0x05, 5 */
    { 0xC7, 0xC7, 0xC7 }, /* 0x0C, 0x06, 6 */
    { 0xED, 0xED, 0xED }, /* 0x0E, 0x07, 7 */
    { 0x19, 0x92, 0x00 }, /* 0x10, 0x08, 8 */
    { 0x3A, 0x1F, 0x00 }, /* 0x12, 0x09, 9 */
    { 0x5D, 0x41, 0x00 }, /* 0x14, 0x0A, 10 */
    { 0x82, 0x64, 0x00 }, /* 0x16, 0x0B, 11 */
    { 0xA7, 0x88, 0x00 }, /* 0x18, 0x0C, 12 */
    { 0xCC, 0xAD, 0x00 }, /* 0x1A, 0x0D, 13 */
    { 0xF2, 0xD2, 0x19 }, /* 0x1C, 0x0E, 14 */
    { 0xFE, 0xFA, 0x40 }, /* 0x1E, 0x0F, 15 */
    { 0x37, 0x00, 0x00 }, /* 0x20, 0x10, 16 */
    { 0x5E, 0x08, 0x00 }, /* 0x22, 0x11, 17 */
    { 0x83, 0x27, 0x00 }, /* 0x24, 0x12, 18 */
    { 0xA9, 0x49, 0x00 }, /* 0x26, 0x13, 19 */
    { 0xCF, 0x6C, 0x00 }, /* 0x28, 0x14, 20 */
    { 0xF5, 0x8F, 0x17 }, /* 0x2A, 0x15, 21 */
    { 0xFE, 0xB4, 0x38 }, /* 0x2C, 0x16, 22 */
    { 0xFE, 0xDF, 0x6F }, /* 0x2E, 0x17, 23 */
    { 0x47, 0x00, 0x00 }, /* 0x30, 0x18, 24 */
    { 0x73, 0x00, 0x00 }, /* 0x32, 0x19, 25 */
    { 0x98, 0x13, 0x00 }, /* 0x34, 0x1A, 26 */
    { 0xBE, 0x32, 0x16 }, /* 0x36, 0x1B, 27 */
    { 0xE4, 0x53, 0x35 }, /* 0x38, 0x1C, 28 */
    { 0xFE, 0x76, 0x57 }, /* 0x3A, 0x1D, 29 */
    { 0xFE, 0x9C, 0x81 }, /* 0x3C, 0x1E, 30 */
    { 0xFE, 0xC6, 0xBB }, /* 0x3E, 0x1F, 31 */
    { 0x44, 0x00, 0x08 }, /* 0x40, 0x20, 32 */
    { 0x6F, 0x00, 0x1F }, /* 0x42, 0x21, 33 */
    { 0x96, 0x06, 0x40 }, /* 0x44, 0x22, 34 */
    { 0xBB, 0x24, 0x62 }, /* 0x46, 0x23, 35 */
    { 0xE1, 0x45, 0x85 }, /* 0x48, 0x24, 36 */
    { 0xFE, 0x67, 0xAA }, /* 0x4A, 0x25, 37 */
    { 0xFE, 0x8C, 0xD6 }, /* 0x4C, 0x26, 38 */
    { 0xFE, 0xB7, 0xF6 }, /* 0x4E, 0x27, 39 */
    { 0x2D, 0x00, 0x4A }, /* 0x50, 0x28, 40 */
    { 0x57, 0x00, 0x67 }, /* 0x52, 0x29, 41 */
    { 0x7D, 0x05, 0x8C }, /* 0x54, 0x2A, 42 */
    { 0xA1, 0x22, 0xB1 }, /* 0x56, 0x2B, 43 */
    { 0xC7, 0x43, 0xD7 }, /* 0x58, 0x2C, 44 */
    { 0xED, 0x65, 0xFE }, /* 0x5A, 0x2D, 45 */
    { 0xFE, 0x8A, 0xF6 }, /* 0x5C, 0x2E, 46 */
    { 0xFE, 0xB5, 0xF7 }, /* 0x5E, 0x2F, 47 */
    { 0x0D, 0x00, 0x82 }, /* 0x60, 0x30, 48 */
    { 0x33, 0x00, 0xA2 }, /* 0x62, 0x31, 49 */
    { 0x55, 0x0F, 0xC9 }, /* 0x64, 0x32, 50 */
    { 0x78, 0x2D, 0xF0 }, /* 0x66, 0x33, 51 */
    { 0x9C, 0x4E, 0xFE }, /* 0x68, 0x34, 52 */
    { 0xC3, 0x72, 0xFE }, /* 0x6A, 0x35, 53 */
    { 0xEB, 0x98, 0xFE }, /* 0x6C, 0x36, 54 */
    { 0xFE, 0xC0, 0xF9 }, /* 0x6E, 0x37, 55 */
    { 0x00, 0x00, 0x91 }, /* 0x70, 0x38, 56 */
    { 0x0A, 0x05, 0xBD }, /* 0x72, 0x39, 57 */
    { 0x28, 0x22, 0xE4 }, /* 0x74, 0x3A, 58 */
    { 0x48, 0x42, 0xFE }, /* 0x76, 0x3B, 59 */
    { 0x6B, 0x64, 0xFE }, /* 0x78, 0x3C, 50 */
    { 0x90, 0x8A, 0xFE }, /* 0x7A, 0x3D, 51 */
    { 0xB7, 0xB0, 0xFE }, /* 0x7C, 0x3E, 52 */
    { 0xDF, 0xD8, 0xFE }, /* 0x7E, 0x3F, 53 */
    { 0x00, 0x00, 0x72 }, /* 0x80, 0x40, 54 */
    { 0x00, 0x1C, 0xAB }, /* 0x82, 0x41, 55 */
    { 0x03, 0x3C, 0xD6 }, /* 0x84, 0x42, 56 */
    { 0x20, 0x5E, 0xFD }, /* 0x86, 0x43, 57 */
    { 0x40, 0x81, 0xFE }, /* 0x88, 0x44, 58 */
    { 0x64, 0xA6, 0xFE }, /* 0x8A, 0x45, 59 */
    { 0x89, 0xCE, 0xFE }, /* 0x8C, 0x46, 60 */
    { 0xB0, 0xF6, 0xFE }, /* 0x8E, 0x47, 61 */
    { 0x00, 0x10, 0x3A }, /* 0x90, 0x48, 62 */
    { 0x00, 0x31, 0x6E }, /* 0x92, 0x49, 63 */
    { 0x00, 0x55, 0xA2 }, /* 0x94, 0x4A, 64 */
    { 0x05, 0x79, 0xC8 }, /* 0x96, 0x4B, 65 */
    { 0x23, 0x9D, 0xEE }, /* 0x98, 0x4C, 66 */
    { 0x44, 0xC2, 0xFE }, /* 0x9A, 0x4D, 67 */
    { 0x68, 0xE9, 0xFE }, /* 0x9C, 0x4E, 68 */
    { 0x8F, 0xFE, 0xFE }, /* 0x9E, 0x4F, 69 */
    { 0x00, 0x1F, 0x02 }, /* 0xA0, 0x50, 70 */
    { 0x00, 0x43, 0x26 }, /* 0xA2, 0x51, 71 */
    { 0x00, 0x69, 0x57 }, /* 0xA4, 0x52, 72 */
    { 0x00, 0x8D, 0x7A }, /* 0xA6, 0x53, 73 */
    { 0x1B, 0xB1, 0x9E }, /* 0xA8, 0x54, 74 */
    { 0x3B, 0xD7, 0xC3 }, /* 0xAA, 0x55, 75 */
    { 0x00, 0x43, 0x26 }, /* 0xAC, 0x56, 76 */
    { 0x86, 0xFE, 0xFE }, /* 0xAE, 0x57, 77 */
    { 0x00, 0x24, 0x03 }, /* 0xB0, 0x58, 78 */
    { 0x00, 0x4A, 0x05 }, /* 0xB2, 0x59, 79 */
    { 0x00, 0x70, 0x0C }, /* 0xB4, 0x5A, 80 */
    { 0x09, 0x95, 0x2B }, /* 0xB6, 0x5B, 81 */
    { 0x28, 0xBA, 0x4C }, /* 0xB8, 0x5C, 82 */
    { 0x49, 0xE0, 0x6E }, /* 0xBA, 0x5D, 83 */
    { 0x6C, 0xFE, 0x92 }, /* 0xBC, 0x5E, 84 */
    { 0x97, 0xFE, 0xB5 }, /* 0xBE, 0x5F, 85 */
    { 0x00, 0x21, 0x02 }, /* 0xC0, 0x60, 86 */
    { 0x00, 0x46, 0x04 }, /* 0xC2, 0x61, 87 */
    { 0x08, 0x6B, 0x00 }, /* 0xC4, 0x62, 88 */
    { 0x28, 0x90, 0x00 }, /* 0xC6, 0x63, 89 */
    { 0x49, 0xB5, 0x09 }, /* 0xC8, 0x64, 90 */
    { 0x6B, 0xDB, 0x28 }, /* 0xCA, 0x65, 91 */
    { 0x8F, 0xFE, 0x49 }, /* 0xCC, 0x66, 92 */
    { 0xBB, 0xFE, 0x69 }, /* 0xCE, 0x67, 93 */
    { 0x00, 0x15, 0x01 }, /* 0xD0, 0x68, 94 */
    { 0x10, 0x36, 0x00 }, /* 0xD2, 0x69, 95 */
    { 0x30, 0x59, 0x00 }, /* 0xD4, 0x6A, 96 */
    { 0x53, 0x7E, 0x00 }, /* 0xD6, 0x6B, 97 */
    { 0x76, 0xA3, 0x00 }, /* 0xD8, 0x6C, 98 */
    { 0x9A, 0xC8, 0x00 }, /* 0xDA, 0x6D, 99 */
    { 0xBF, 0xEE, 0x1E }, /* 0xDC, 0x6E, 100 */
    { 0xE8, 0xFE, 0x3E }, /* 0xDE, 0x6F, 101 */
    { 0x1A, 0x02, 0x00 }, /* 0xE0, 0x70, 102 */
    { 0x3B, 0x1F, 0x00 }, /* 0xE2, 0x71, 103 */
    { 0x5E, 0x41, 0x00 }, /* 0xE4, 0x72, 104 */
    { 0x83, 0x64, 0x00 }, /* 0xE6, 0x73, 105 */
    { 0xA8, 0x88, 0x00 }, /* 0xE8, 0x74, 106 */
    { 0xCE, 0xAD, 0x00 }, /* 0xEA, 0x75, 107 */
    { 0xF4, 0xD2, 0x18 }, /* 0xEC, 0x76, 108 */
    { 0xFE, 0xFA, 0x40 }, /* 0xEE, 0x77, 109 */
    { 0x38, 0x00, 0x00 }, /* 0xF0, 0x78, 110 */
    { 0x5F, 0x08, 0x00 }, /* 0xF2, 0x79, 111 */
    { 0x84, 0x27, 0x00 }, /* 0xF4, 0x7A, 112 */
    { 0xAA, 0x49, 0x00 }, /* 0xF6, 0x7B, 113 */
    { 0xD0, 0x6B, 0x00 }, /* 0xF8, 0x7C, 114 */
    { 0xF6, 0x8F, 0x18 }, /* 0xFA, 0x7D, 115 */
    { 0xFE, 0xB4, 0x39 }, /* 0xFC, 0x7E, 116 */
    { 0xFE, 0xDF, 0x70 }, /* 0xFE, 0x7F, 117 */
    { 0x00, 0x00, 0x00 }, /* 0x00, 0x00, 0 */
    { 0x1A, 0x1A, 0x1A }, /* 0x02, 0x01, 1 */
    { 0x39, 0x39, 0x39 }, /* 0x04, 0x02, 2 */
    { 0x58, 0x58, 0x58 }, /* 0x06, 0x03, 3 */
    { 0x7E, 0x7E, 0x7E }, /* 0x08, 0x04, 4 */
    { 0xA2, 0xA2, 0xA2 }, /* 0x0A, 0x05, 5 */
    { 0xC7, 0xC7, 0xC7 }, /* 0x0C, 0x06, 6 */
    { 0xED, 0xED, 0xED }, /* 0x0E, 0x07, 7 */
    { 0x19, 0x92, 0x00 }, /* 0x10, 0x08, 8 */
    { 0x3A, 0x1F, 0x00 }, /* 0x12, 0x09, 9 */
    { 0x5D, 0x41, 0x00 }, /* 0x14, 0x0A, 10 */
    { 0x82, 0x64, 0x00 }, /* 0x16, 0x0B, 11 */
    { 0xA7, 0x88, 0x00 }, /* 0x18, 0x0C, 12 */
    { 0xCC, 0xAD, 0x00 }, /* 0x1A, 0x0D, 13 */
    { 0xF2, 0xD2, 0x19 }, /* 0x1C, 0x0E, 14 */
    { 0xFE, 0xFA, 0x40 }, /* 0x1E, 0x0F, 15 */
    { 0x37, 0x00, 0x00 }, /* 0x20, 0x10, 16 */
    { 0x5E, 0x08, 0x00 }, /* 0x22, 0x11, 17 */
    { 0x83, 0x27, 0x00 }, /* 0x24, 0x12, 18 */
    { 0xA9, 0x49, 0x00 }, /* 0x26, 0x13, 19 */
    { 0xCF, 0x6C, 0x00 }, /* 0x28, 0x14, 20 */
    { 0xF5, 0x8F, 0x17 }, /* 0x2A, 0x15, 21 */
    { 0xFE, 0xB4, 0x38 }, /* 0x2C, 0x16, 22 */
    { 0xFE, 0xDF, 0x6F }, /* 0x2E, 0x17, 23 */
    { 0x47, 0x00, 0x00 }, /* 0x30, 0x18, 24 */
    { 0x73, 0x00, 0x00 }, /* 0x32, 0x19, 25 */
    { 0x98, 0x13, 0x00 }, /* 0x34, 0x1A, 26 */
    { 0xBE, 0x32, 0x16 }, /* 0x36, 0x1B, 27 */
    { 0xE4, 0x53, 0x35 }, /* 0x38, 0x1C, 28 */
    { 0xFE, 0x76, 0x57 }, /* 0x3A, 0x1D, 29 */
    { 0xFE, 0x9C, 0x81 }, /* 0x3C, 0x1E, 30 */
    { 0xFE, 0xC6, 0xBB } /* 0x3E, 0x1F, 31 */
};
#endif /* COLOUR_TEST */

/* Resets the TIA instance to default conditions with no state set.
 */
void TIA_init(void)
{
    int i = 0;
    /* Data registers */
    for (i=0; i<TIA_WRITE_REG_LEN; i++) {
        tia.write_regs[i] = 0;
    }
    for (i=0; i<TIA_READ_REG_LEN; i++) {
        tia.read_regs[i] = 0;
    }
    for (i=0; i<TIA_COLOUR_CLOCK_VISIBLE; i++) {
        tia_line_buffer[i] = (tia_pixel_t){0};
    }
    tia.hmove_set = 0;
    tia.missiles[0] = (tia_missile_t){0};
    tia.missiles[1] = (tia_missile_t){0};
    tia.players[0] = (tia_player_t){0};
    tia.players[1] = (tia_player_t){0};
}

/* Retrieves a value in a specified register
 *
 * reg: register to read (e.g., VSYNC, VBLANK, etc ...).
 * *value: location to place retrieved value of register into.
 */
void TIA_read_register(uint8_t reg, uint8_t *value)
{
    *value = tia.read_regs[reg];
}

/* Writes a value into a register location
 *
 * reg: register to write to (e.g., CXM0P, CXM1P etc ...) value: value to place into register.
 */
void TIA_write_register(uint8_t reg, uint8_t value)
{
    /* Perform special state logic on strobing registers which influence
     * state regardless of value written. E.g., writing a 0 to WSYNC still
     * results in the processor clock suspending
     */
    switch (reg) {
        case TIA_WRITE_REG_COLUBK:
            tia.write_regs[reg] = value;
            break;
        case TIA_WRITE_REG_PF0:
            /* Intentional fallthrough */
        case TIA_WRITE_REG_PF1:
            /* Intentional fallthrough */
        case TIA_WRITE_REG_PF2:
            /* Intentional fallthrough */
        case TIA_WRITE_REG_CTRLPF:
            tia.write_regs[reg] = value;
            TIA_update_playfield();
            break;
        case TIA_WRITE_REG_WSYNC:
            tia.write_regs[TIA_WRITE_REG_WSYNC] = 1;
            break;
        case TIA_WRITE_REG_RSYNC:
            tia.colour_clock = 0;
            break;
        case TIA_WRITE_REG_RESP0:
            TIA_reset_player(0);
            break;
        case TIA_WRITE_REG_RESP1:
            TIA_reset_player(1);
            break;
        case TIA_WRITE_REG_RESM0:
            TIA_reset_missile(0);
            break;
        case TIA_WRITE_REG_RESM1:
            TIA_reset_missile(1);
            break;
        case TIA_WRITE_REG_RESBL:
            break;
        case TIA_WRITE_REG_GRP0:
            tia.write_regs[reg] = value;
            TIA_update_player_buffer(0);
            break;
        case TIA_WRITE_REG_GRP1:
            tia.write_regs[reg] = value;
            TIA_update_player_buffer(1);
            break;
        case TIA_WRITE_REG_HMOVE:
            if (tia.colour_clock < TIA_COLOUR_CLOCK_HSYNC) {
                tia.hmove_set = 1;
                TIA_update_player_buffer(0);
                TIA_update_player_buffer(1);
                TIA_update_missile_buffer(0);
                TIA_update_missile_buffer(1);
            }
            break;
        case TIA_WRITE_REG_ENAM0:
            tia.write_regs[reg] = value;
            TIA_update_missile_buffer(0);
            break;
        case TIA_WRITE_REG_ENAM1:
            tia.write_regs[reg] = value;
            TIA_update_missile_buffer(1);
            break;
        case TIA_WRITE_REG_HMP0:
            tia.write_regs[reg] = value;
            TIA_update_player_buffer(0);
            break;
        case TIA_WRITE_REG_HMP1:
            tia.write_regs[reg] = value;
            TIA_update_player_buffer(1);
            break;
        case TIA_WRITE_REG_NUSIZ0:
            tia.write_regs[reg] = value;
            TIA_update_missile_buffer(0);
            break;
        case TIA_WRITE_REG_NUSIZ1:
            tia.write_regs[reg] = value;
            TIA_update_missile_buffer(1);
            break;
        case TIA_WRITE_REG_HMCLR:
            tia.hmove_set = 0;
            TIA_update_player_buffer(0);
            TIA_update_player_buffer(1);
            TIA_update_missile_buffer(0);
            TIA_update_missile_buffer(1);
            break;
        case TIA_WRITE_REG_CXCLR:
            /* Reset all collision latches*/
            tia.read_regs[TIA_READ_REG_CXM0P] = 0;
            tia.read_regs[TIA_READ_REG_CXM1P] = 0;
            tia.read_regs[TIA_READ_REG_CXP0FB] = 0;
            tia.read_regs[TIA_READ_REG_CXP1FB] = 0;
            tia.read_regs[TIA_READ_REG_CXM0FB] = 0;
            tia.read_regs[TIA_READ_REG_CXM1FB] = 0;
            tia.read_regs[TIA_READ_REG_CXBLPF] = 0;
            tia.read_regs[TIA_READ_REG_CXPPMM] = 0;
            break;
        default:
            tia.write_regs[reg] = value;
    }
}

void TIA_reset_player(uint8_t player)
{
    tia.players[player].scanline_reset = 1;
    tia.players[player].position_clock = 0;
    if (tia.colour_clock > TIA_COLOUR_CLOCK_HSYNC) {
        tia.players[player].position_clock = tia.colour_clock-TIA_COLOUR_CLOCK_HSYNC;
    }
    TIA_update_player_buffer(player);
}

void TIA_update_player_buffer(uint8_t player)
{
    int position, mirror, pattern, i, pixel_clock, size_mask, draw_count;
    tia_writable_register_t reflect_reg, graphics_reg, offset_reg, vertical_reg, size_reg;

    TIA_reset_line_buffer(tia.players[player].line_buffer);
    TIA_get_player_registers(player, &reflect_reg, &graphics_reg, &offset_reg, &vertical_reg, &size_reg);

    mirror = (tia.write_regs[reflect_reg] & 0x8) ? 1 : 0;

    position = tia.players[player].position_clock;
    if (tia.hmove_set) {
        TIA_apply_HMOVE(offset_reg, &position);
    }

    if (mirror) {
        pattern = 0;
        for (i=0; i<8; i++) {
            pattern |= (tia.write_regs[graphics_reg] & (1 << i)) ? (1 << i) : 0;
        }
    } else {
        pattern = tia.write_regs[graphics_reg];
    }

    size_mask = tia_player_size_map[(tia.write_regs[size_reg] & 0x7)];
    draw_count = 8;
    pixel_clock = 0;

    for (i=0; i<TIA_COLOUR_CLOCK_VISIBLE; i++) {
        if (i >= position) {
            if ((draw_count > -1) && (size_mask & (1 << draw_count))) {
                tia.players[player].line_buffer[i] = (pattern & (1 << pixel_clock) ? 1 : 0);
            }
            /* Every 8 clock cycles reset and start testing bits over */
            pixel_clock++;
            if (pixel_clock >= 8) {
                draw_count--;
                pixel_clock = 0;
            }
        }
    }
}

void TIA_get_player_registers(uint8_t player, tia_writable_register_t *reflect,
        tia_writable_register_t *graphics, tia_writable_register_t *offset,
        tia_writable_register_t *vertical, tia_writable_register_t *size)
{
    player ? (*reflect = TIA_WRITE_REG_REFP1)   : (*reflect = TIA_WRITE_REG_REFP0);
    player ? (*graphics = TIA_WRITE_REG_GRP1)   : (*graphics = TIA_WRITE_REG_GRP0);
    player ? (*offset = TIA_WRITE_REG_HMP1)     : (*offset = TIA_WRITE_REG_HMP0);
    player ? (*vertical = TIA_WRITE_REG_VDELP1) : (*vertical = TIA_WRITE_REG_VDELP0);
    player ? (*size = TIA_WRITE_REG_NUSIZ1)     : (*size = TIA_WRITE_REG_NUSIZ0);
}

void TIA_get_missile_registers(uint8_t missile, tia_writable_register_t *enable,
        tia_writable_register_t *size, tia_writable_register_t *offset)
{
    missile ? (*enable = TIA_WRITE_REG_ENAM1) : (*enable = TIA_WRITE_REG_ENAM0);
    missile ? (*size = TIA_WRITE_REG_NUSIZ1)  : (*size = TIA_WRITE_REG_NUSIZ0);
    missile ? (*offset = TIA_WRITE_REG_HMM1)  : (*offset = TIA_WRITE_REG_HMM0);
}

void TIA_reset_missile(uint8_t missile)
{
    tia.missiles[missile].scanline_reset = 1;
    tia.missiles[missile].position_clock = 0;
    if (tia.colour_clock > TIA_COLOUR_CLOCK_HSYNC) {
        tia.missiles[missile].position_clock = tia.colour_clock-TIA_COLOUR_CLOCK_HSYNC;
    }
    TIA_update_missile_buffer(missile);
}

void TIA_update_missile_buffer(uint8_t missile)
{
    int position, i;
    tia_writable_register_t enable_reg, size_reg, offset_reg;

    TIA_reset_line_buffer(tia.missiles[missile].line_buffer);
    TIA_get_missile_registers(missile, &enable_reg, &size_reg, &offset_reg);

    if (!tia.write_regs[enable_reg]) {
        return;
    }

    position = tia.missiles[missile].position_clock;
    tia.missiles[missile].width = (1 << (tia.write_regs[size_reg] >> 4));
    if (tia.hmove_set) {
        TIA_apply_HMOVE(offset_reg, &position);
    }

    for (i=0; i<TIA_COLOUR_CLOCK_VISIBLE; i++) {
        if ((i >= position) && (i <= (position + tia.missiles[missile].width))) {
            tia.missiles[missile].line_buffer[i] = 1;
        }
    }
}

void TIA_get_playfield_pattern(uint32_t *playfield)
{
    *playfield = 0;
    *playfield |= (tia.write_regs[TIA_WRITE_REG_PF0] >> 4);
    *playfield |= (tia.write_regs[TIA_WRITE_REG_PF1] << 4);
    *playfield |= (tia.write_regs[TIA_WRITE_REG_PF2] << 12);
}

void TIA_update_playfield()
{
    uint32_t pattern, i;
    TIA_get_playfield_pattern(&pattern);
    tia.playfield.mirror_enable = (tia.write_regs[TIA_WRITE_REG_CTRLPF] & 0x01) ? 1 : 0;
    /* Fill in the first half of the screen */
    for (i=0; i<TIA_COLOUR_CLOCK_VISIBLE_HALF; i++) {
        /* N.B divide by four as each playfield bit covers four TIA clock cycles */
        tia.playfield.line_buffer[i] = ((pattern & (1 << (i/4))) ? 1 : 0);
    }
    /* Now fill in the second-half, compensating if mirroring has been enabled */
    for (i=0; i<TIA_COLOUR_CLOCK_VISIBLE_HALF; i++) {
        if (tia.playfield.mirror_enable) {
            tia.playfield.line_buffer[i+TIA_COLOUR_CLOCK_VISIBLE_HALF] = ((pattern & (0x80000 >> (i/4))) ? 1 : 0);
        } else {
            tia.playfield.line_buffer[i+TIA_COLOUR_CLOCK_VISIBLE_HALF] = ((pattern & (1 << (i/4))) ? 1 : 0);
        }
    }
}

int TIA_test_playfield_bit()
{
    if (tia.colour_clock < TIA_COLOUR_CLOCK_HSYNC) {
        return 0;
    }
    return (tia.playfield.line_buffer[tia.colour_clock - TIA_COLOUR_CLOCK_HSYNC] ? 1 : 0);
}

int TIA_test_missile_bit(uint8_t missile)
{
    if (tia.colour_clock < TIA_COLOUR_CLOCK_HSYNC) {
        return 0;
    }
    return (tia.missiles[missile].line_buffer[tia.colour_clock - TIA_COLOUR_CLOCK_HSYNC] ? 1 : 0);
}

int TIA_test_player_bit(uint8_t player)
{
    if (tia.colour_clock < TIA_COLOUR_CLOCK_HSYNC) {
        return 0;
    }
    return (tia.players[player].line_buffer[tia.colour_clock - TIA_COLOUR_CLOCK_HSYNC] ? 1 : 0);
}

void TIA_apply_HMOVE(tia_writable_register_t offset_reg, int *position)
{
    uint8_t offset = (tia.write_regs[offset_reg] >> 4);
    if (offset & 0x8) {
        *position -= (offset & 0x7) + 1;
    } else {
        *position += offset;
    }
    if (*position < 0) *position = 0;
}

uint8_t TIA_reverse_byte(uint8_t byte)
{
    byte = (byte & 0xF0) >> 4 | (byte & 0x0F) << 4;
    byte = (byte & 0xCC) >> 2 | (byte & 0x33) << 2;
    byte = (byte & 0xAA) >> 1 | (byte & 0x55) << 1;
    return byte;
}

void TIA_generate_colour(void)
{
    /* Grab the background. If there's an element on the same clock count
     * we'll overwrite it
     */
    tia_pixel_t pixel = {0};
    TIA_colour_to_RGB(tia.write_regs[TIA_WRITE_REG_COLUBK], &pixel);

    /* TODO check order of priority established in PFB bits
     * to establish if playfield need to be rendered over player
     * objects
     */
    if (tia.write_regs[TIA_WRITE_REG_CTRLPF] & 0x04) {
        /* Control register is specifying that priority be remapped to:
         * Highest: PF, BL
         * Second:  P0, M0
         * Third:   P1, M1
         * Lowest:  BK
         */
        if (TIA_test_missile_bit(1)) {
            TIA_colour_to_RGB(tia.write_regs[TIA_WRITE_REG_COLUP0], &pixel);
        }
        if (TIA_test_player_bit(1)) {
            TIA_colour_to_RGB(tia.write_regs[TIA_WRITE_REG_COLUP1], &pixel);
        }
        if (TIA_test_missile_bit(0)) {
            TIA_colour_to_RGB(tia.write_regs[TIA_WRITE_REG_COLUP0], &pixel);
        }
        if (TIA_test_player_bit(0)) {
            TIA_colour_to_RGB(tia.write_regs[TIA_WRITE_REG_COLUP0], &pixel);
        }
        if (TIA_test_playfield_bit()) {
            TIA_colour_to_RGB(tia.write_regs[TIA_WRITE_REG_COLUPF], &pixel);
        }
    } else {
        /* Default priority control:
         * Highest: P0, M0
         * Second:  P1, M1
         * Third:   PF, BL
         * Lowest:  BK
         */
        if (TIA_test_playfield_bit()) {
            TIA_colour_to_RGB(tia.write_regs[TIA_WRITE_REG_COLUPF], &pixel);
        }
        if (TIA_test_missile_bit(1)) {
            TIA_colour_to_RGB(tia.write_regs[TIA_WRITE_REG_COLUP1], &pixel);
        }
        if (TIA_test_player_bit(1)) {
            TIA_colour_to_RGB(tia.write_regs[TIA_WRITE_REG_COLUP1], &pixel);
        }
        if (TIA_test_missile_bit(0)) {
            TIA_colour_to_RGB(tia.write_regs[TIA_WRITE_REG_COLUP0], &pixel);
        }
        if (TIA_test_player_bit(0)) {
            TIA_colour_to_RGB(tia.write_regs[TIA_WRITE_REG_COLUP0], &pixel);
        }
    }

    TIA_write_to_buffer(pixel, (tia.colour_clock-TIA_COLOUR_CLOCK_HSYNC));

    /* TODO check for collisions and set registers appropriately */
}

void TIA_write_to_buffer(tia_pixel_t pixel, int pixel_index)
{
    if (pixel_index < TIA_COLOUR_CLOCK_VISIBLE) {
        tia_line_buffer[pixel_index].R = pixel.R;
        tia_line_buffer[pixel_index].G = pixel.G;
        tia_line_buffer[pixel_index].B = pixel.B;
    }
}

void TIA_colour_to_RGB(uint8_t tia_colour, tia_pixel_t* pixel)
{
    int index = tia_colour >> 1;
    pixel->R = tia_colour_map[index].R;
    pixel->G = tia_colour_map[index].G;
    pixel->B = tia_colour_map[index].B;
}

int TIA_clock_tick()
{
    int i;
    /* Reset colour clock and prepare begin next line */
    if (tia.colour_clock >= TIA_COLOUR_CLOCK_TOTAL) {
        tia.colour_clock = 0;
        tia.write_regs[TIA_WRITE_REG_WSYNC] = 0;
        tia.missiles[0].scanline_reset = 0;
        tia.missiles[1].scanline_reset = 0;
        tia.hmove_set = 0;
        tia.write_regs[TIA_WRITE_REG_HMOVE] = 0;
        return 0;
    }
    if (tia.colour_clock == TIA_COLOUR_CLOCK_HSYNC) {
        TIA_update_player_buffer(0);
        TIA_update_player_buffer(1);
        TIA_update_missile_buffer(0);
        TIA_update_missile_buffer(1);
    }
    if (tia.colour_clock > TIA_COLOUR_CLOCK_HSYNC) {
        TIA_generate_colour();
    } else {
        /* Horizontal or vertical sync time, no need to generate a colour */
    }
    tia.colour_clock++;
    return tia.colour_clock;
}

int TIA_draw_line(int line_count)
{
    ili9341_draw_line(tia_line_buffer, line_count, ATARI_RESOLUTION_WIDTH);
}

int TIA_get_WSYNC()
{
    return (tia.write_regs[TIA_WRITE_REG_WSYNC] ? 1 : 0);
}

int TIA_get_VSYNC()
{
    return (tia.write_regs[TIA_WRITE_REG_VSYNC] ? 1 : 0);
}

int TIA_get_VBLANK()
{
    return (tia.write_regs[TIA_WRITE_REG_VBLANK] ? 1 : 0);
}

void TIA_reset_line_buffer(uint8_t line_buffer[])
{
    int i;
    for (i=0; i<TIA_COLOUR_CLOCK_VISIBLE; i++) {
        line_buffer[i] = 0;
    }
}

int TIA_reset_buffer()
{
    int i;
    for (i=0; i<ATARI_RESOLUTION_WIDTH; i++) {
        tia_line_buffer[i].R = 0x0;
        tia_line_buffer[i].G = 0x0;
        tia_line_buffer[i].B = 0x0;
    }
}
