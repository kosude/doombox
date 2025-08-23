/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

#include "puts.h"
#include "../vga.h"
#include "font.h"

// Position values of the 'put head' a.k.a. current coordinate offset in vram.
static uint16_t _head_x = 0;
static uint16_t _head_y = 0;

void
vga_putc(const char ch, const uint8_t col)
{
    // if LF then just advance head to the next line
    if (ch == 10) {
        _head_x = 0;
        _head_y += 8;
        return;
    }

    vga_fontat(ch, col, _head_x, _head_y);

    // increment head
    if (_head_x < VGA_WIDTH - 8) {
        _head_x += 8;
    } else {
        // advance to next line if at the end
        _head_x = 0;
        _head_y += 8;
    }
}
