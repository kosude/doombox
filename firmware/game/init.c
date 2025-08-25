/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

#include "kernel/drivers/vga/stress.h"
#include "kernel/drivers/vga/text/puts.h"
#include "kernel/drivers/vga/vram.h"
#include "kernel/panic.h"

#include "renderer/primitive.h"

void
game_main(void)
{
    // render_prim_line(20, 10, 70, 30, 0xFF);
    // vram_swap();
    // for (;;) {
    // }

    // game main loop
    for (;;) {
        vram_clear(0x0);

        render_prim_line(0, 0, 320, 200, 0xFF);
        render_prim_line(320, 0, 0, 200, 0xFF);

        // vga_stress_grid(20, 10, 0b00011000);

        // vga_padvance(7);
        // vga_puts("      *** DOOMbox puts() test ***\n\n\0", 0x0);
        // vga_puts("          (very cool)\0", 0x0);
        // vga_padvance(4);
        // vga_puts("Now in 320x200 resolution!!\n\0", 0x0);
        // vga_puts("(and with double buffering)\0", 0x0);

        // vga_stress_atlas(0xFF);

        vram_swap();
    }
}
