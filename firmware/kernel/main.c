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

    vga_pset(0, 168);
    vga_puts("*** DOOMbox VGA palette test ***\n\n\0", 0xFF);
    vga_puts("    (very cool)\0", 0xFF);

    return 0;
}
