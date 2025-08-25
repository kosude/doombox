/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

#include "audio/pz_act.h"
#include "vga/vga.h"

// Pico GPIO pin for the onboard speaker
#define GPIO_OBSPKR 16

// Pico GPIOs for video output
#define GPIO_B2G3R3_BASE 2
#define GPIO_HSYNC       10
#define GPIO_VSYNC       11

bool
hal_init(void)
{
    // async obspkr interface
    pz_act_init(GPIO_OBSPKR);

    // vga graphics driver
    vga_init((struct vga_config){
        .gpio_b2g3r3_base = GPIO_B2G3R3_BASE,
        .gpio_hsync = GPIO_HSYNC,
        .gpio_vsync = GPIO_VSYNC,
    });

    return true;
}
