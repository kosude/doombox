/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

#include <stddef.h>

#include "text/puts.h"
#include "vga.h"
#include "vram.h"

void
vga_stress_palette(void)
{
    size_t posx = 0, posy = 0;

    // loop through blocks (incr red)
    for (uint8_t r = 0; r < 8; r++) {
        // loop through rows (incr green)
        for (uint8_t g = 0; g < 8; g++) {
            // loop through columns (inc blue)
            for (uint8_t b = 0; b < 4; b++) {
                // encode rgb value (BBGGGRRR)
                uint8_t rgb = (b << 6) | (g << 3) | r;

                // draw a 20x10 block of colour rgb
                for (size_t i = 0; i < 10; i++) {
                    for (size_t j = 0; j < 20; j++) {
                        ((uint8_t *)
                             VRAM)[((posy + i) * VGA_WIDTH) + (posx + j)] =
                            rgb;
                    }
                }

                posx += 20;
            }
            posx -= 80;
            posy += 10;
        }

        if (r != 3) {
            posx += 80;
            posy -= 80;
        } else {
            posx = 0;
        }
    }
}

void
vga_stress_grid(const uint16_t xinc, const uint16_t yinc, const uint8_t col)
{
    for (uint16_t x = 0; x < VGA_WIDTH; x++) {
        // top and bottom rows
        VRAM[x] = VRAM[((VGA_HEIGHT - 1) * VGA_WIDTH) + x] = col;

        for (uint16_t y = 0; y < VGA_HEIGHT; y++) {
            // left and right columns
            VRAM[y * VGA_WIDTH] = VRAM[y * VGA_WIDTH - 1] = col;

            // vertical lines
            if (x % xinc == 0) {
                VRAM[y * VGA_WIDTH + x] = col;
            }
            // horizontal lines
            if (y % yinc == 0) {
                VRAM[y * VGA_WIDTH + x] = col;
            }
        }
    }
}

void
vga_stress_atlas(const uint8_t col)
{
    vga_pset(0, 0);
    for (uint8_t i = 0; i < 128; i++) {
        vga_putc(i, col);
    }
}
