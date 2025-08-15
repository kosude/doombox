/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

#include <hardware/gpio.h>

#include "pz_act.h"

void
pz_act_init(const uint8_t gpio)
{
    gpio_init(gpio);
    gpio_set_dir(gpio, GPIO_OUT);
}
