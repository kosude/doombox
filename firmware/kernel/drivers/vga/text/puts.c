/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

#include "puts.h"
#include "../vga.h"
#include "font.h"
#include "kstdio.h"

// Position values of the 'put head' a.k.a. current coordinate offset in vram.
static struct {
    uint16_t x;
    uint16_t y;
} _head = { 0 };

void
vga_putc(const char ch, const uint8_t col)
{
    // if LF then just advance head to the next line
    if (ch == 10) {
        _head.x = 0;
        _head.y += 8;
        return;
    }

    vga_fontat(ch, col, _head.x, _head.y);

    // increment head
    if (_head.x < VGA_WIDTH - 8) {
        _head.x += 8;
    } else {
        // advance to next line if at the end
        _head.x = 0;
        _head.y += 8;
    }
}

void
vga_padvance(void)
{
    _head.x = 0;
    _head.y += 8;
}

void
vga_pincrement(const uint16_t n)
{
    _head.x += 8 * n;
    _head.y += 8 * (_head.x / VGA_WIDTH);
    _head.x %= VGA_WIDTH;
}

void
vga_pset(const uint16_t x, const uint16_t y)
{
    _head.x = x;
    _head.y = y;
}
