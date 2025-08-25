/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

#include "drivers/hal.h"
#include "drivers/vga/stress.h"
#include "drivers/vga/text/puts.h"
#include "game/init.h"
#include "kstdio.h"

int
main(void)
{
    kstdio_init(); // no-op in release config
    hal_init();

    game_main();

    return 0;
}
