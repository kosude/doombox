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

// TMP
#include <hardware/gpio.h>
#include <pico/stdlib.h>

bool
hal_init(void)
{
    // TODO TMP
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    pz_act_init(GPIO_OBSPKR);

    // init vga
    struct vga_outputs vga_op = {
        .gpio_rgb8 = 2,
        .gpio_hsync = 10,
        .gpio_vsync = 11,
    };
    vga_init(vga_op);

    return true;
}
