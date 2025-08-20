/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

#include "hal.h"
#include "audio/pz_act.h"
#include "vga/vga.h"

// Pico GPIO pin for the onboard speaker
#define GPIO_OBSPKR 16

// Pico GPIOs for video output
#define GPIO_B2G3R3_BASE 2
#define GPIO_HSYNC       10
#define GPIO_VSYNC       11

// Reserved PIO instance(s)
#define PIO_VGA 0

bool
hal_init(void)
{
    pz_act_init(GPIO_OBSPKR);

    // init vga
    struct vga_config vga_cfg = {
        .pio = PIO_VGA,
        .gpio_b2g3r3_base = GPIO_B2G3R3_BASE,
        .gpio_hsync = GPIO_HSYNC,
        .gpio_vsync = GPIO_VSYNC,
    };
    vga_init(vga_cfg);

    return true;
}
