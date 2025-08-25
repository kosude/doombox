/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

#include "drivers/hal.h"
#include "drivers/vga/stress.h"
#include "drivers/vga/text/puts.h"
#include "kstdio.h"

int
main(void)
{
    kstdio_init(); // no-op in release config
    hal_init();

    vga_stress_palette();
    vga_stress_grid(32, 100, 0b00011000);
    vga_stress_grid(32, 100, 0b00011000);
    vga_stress_grid(32, 100, 0b00011000);

    vga_pset(0, 0); // reset text put head
    vga_padvance(4);
    vga_puts("      *** DOOMbox puts() test ***\n\n\0", 0xFF);
    vga_puts("          (very cool)\0", 0xFF);
    vga_padvance(7);
    vga_puts("Now in 320x200 resolution!!\n\0", 0xFF);
    vga_puts("(and with double buffering)\0", 0xFF);

    for (;;) {
    }

    return 0;
}
