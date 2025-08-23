/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

/******************************************************************************
 * font.h
 * A baked 8x8 ASCII font for kernel text output in VGA graphics mode.
 * Note: using this font allows for a 40x25 usable text grid (with 320x200
 * output).
 *****************************************************************************/

#pragma once
#ifndef __vga__font_h__
#define __vga__font_h__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

extern uint64_t __vgafont[128];

/**
 * Put an ASCII character `ch` at the specified location on the screen (320x200
 * coordinates). The position is of the top-left pixel in the font character. A
 * colour can be specified using `col`.
 */
void
vga_fontat(const char ch,
           const uint8_t col,
           const uint16_t x,
           const uint16_t y);

#ifdef __cplusplus
}
#endif
#endif
