/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

#include "hal.h"
#include "audio/pz_act.h"
#include "video/vga.h"

// Pico GPIO pin for the onboard speaker
#define GPIO_OBSPKR 16

bool
hal_init(void)
{
    pz_act_init(GPIO_OBSPKR);

    // init vga
    struct vga_outputs vga_op = {
        .gpio_scl0 = 2,
        .gpio_sdo0 = 3,
        .gpio_scsvga = 4,
        .gpio_hsync = 6,
        .gpio_vsync = 7,
    };
    vga_init(vga_op);

    return true;
}
