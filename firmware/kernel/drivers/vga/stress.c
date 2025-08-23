/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

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
