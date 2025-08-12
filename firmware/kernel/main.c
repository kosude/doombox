/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

#include "drivers/hal.h"
#include "panic.h"

int
main(void)
{
    hal_init();

    // just for testing
    kpanic();

    return 0;
}
