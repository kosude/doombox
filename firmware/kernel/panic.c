/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

#include <hardware/sync.h>

#include "drivers/vga/text/puts.h"
#include "drivers/vga/vga.h"
#include "drivers/vga/vram.h"

void
kpanic(void)
{
    vram_clear(0x0);

    vga_pset(132, 20);
    vga_puts("DOOMBOX\0", 0b00000110);

    // backdrop for title
    for (uint16_t y = 38; y < 50; y++) {
        for (uint16_t x = 88; x < 232; x++) {
            VRAM[y * VGA_WIDTH + x] = 0b00000111;
        }
    }
    vga_pset(96, 40);
    vga_puts("! KERNEL PANIC !\0", 0xFF);

    vga_padvance(5);

    // brief description
    vga_puts("  An unhandled exception has occurred.\n"
             "  You will need to reset the system.\0",
             0b00110111);

    // hang the system, equivalent to cli+hlt
    save_and_disable_interrupts();
    __wfi();
}
