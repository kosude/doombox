/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

#include <hardware/sync.h>

#include "kstdio.h"

#include "panic.h"

void
kpanic(void)
{
    //  TODO kernel panic display to VGA

    kprintf("Kernel panics are unimplemented!\n");

    // hang the system, equivalent to cli+hlt
    save_and_disable_interrupts();
    __wfi();
}
