/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

/******************************************************************************
 * puts.h
 * Utility procedures for displaying ASCII characters and strings to the VGA
 * video memory buffer, using the font defined in font.h.
 *****************************************************************************/

#pragma once
#ifndef __text__puts_h__
#define __text__puts_h__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * Write a single ASCII character to the screen, positioning at the current put
 * head. `ch` is the character to write, and `col` is its RGB colour.
 */
void
vga_putc(const char ch, const uint8_t col);

#ifdef __cplusplus
}
#endif
#endif
