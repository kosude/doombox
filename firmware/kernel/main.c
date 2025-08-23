/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

#include "drivers/hal.h"
#include "drivers/vga/text/puts.h"
#include "kstdio.h"

int
main(void)
{
    kstdio_init(); // no-op in release config
    hal_init();

    vga_putc('H', 0b00000111);
    vga_putc('e', 0b00000111);
    vga_putc('l', 0b00000111);
    vga_putc('l', 0b00000111);
    vga_putc('o', 0b00000111);
    vga_putc(' ', 0b00000111);
    vga_putc('W', 0b00000111);
    vga_putc('o', 0b00000111);
    vga_putc('r', 0b00000111);
    vga_putc('l', 0b00000111);
    vga_putc('d', 0b00000111);

    return 0;
}
