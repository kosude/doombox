/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

#include "primitive.h"
#include "kernel/drivers/vga/vga.h"
#include "kernel/drivers/vga/vram.h"

void
render_prim_line(const uint16_t x0,
                 const uint16_t y0,
                 const uint16_t x1,
                 const uint16_t y1,
                 const uint8_t col)
{
    // absolute difference between points
    // TODO optimise (translate for Arm): https://stackoverflow.com/a/11927940
    const uint16_t dx = ((x1 > x0) ? x1 - x0 : x0 - x1);
    const uint16_t dy = ((y1 > y0) ? y1 - y0 : y0 - y1);

    // get sign of dx and dy
    // TODO optimise as in GLOOM:
    // https://gitlab.com/kosude/doom/-/blob/main/game/src/renderer/primitives.asm?ref_type=heads#L69
    const int8_t sx = (x1 > x0) ? 1 : -1;
    const int8_t sy = (y1 > y0) ? 1 : -1;

    int16_t err = ((dx > dy) ? dx : -dy) / 2;
    int16_t e2;

    uint16_t x = x0, y = y0;
    for (;;) {
        // break when x and y at their final values
        if (x == x1 && y == y1) {
            break;
        }

        // check within viewport bounds
        if (x >= 0 && x < VGA_WIDTH && y >= 0 && y < VGA_HEIGHT) {
            VRAM[y * VGA_WIDTH + x] = col;
        }

        e2 = err;
        if (e2 > -dx) {
            err -= dy;
            x += sx;
        }
        if (e2 < dy) {
            err += dx;
            y += sy;
        }
    }
}
