/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

#include <pico/stdlib.h>

#include "drivers/hal.h"
#include "kstdio.h"
#include "panic.h"

int
main(void)
{
    kstdio_init(); // no-op in release config
    hal_init();

    // just for testing
    kpanic();

    return 0;
}
