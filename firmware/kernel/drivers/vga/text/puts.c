/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

#include "../vga.h"
#include "font.h"

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
vga_puts(const char *const str, const uint8_t col)
{
    const char *ch = str;
    while (*ch) {
        vga_putc(*ch, col);
        ch++;
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
    _head.y += 8 * (_head.x / VGA_WIDTH); // advance lines if x is past EOL
    _head.x %= VGA_WIDTH; // reduce x to the offset from start of the line
}

void
vga_pset(const uint16_t x, const uint16_t y)
{
    _head.x = x;
    _head.y = y;
}
