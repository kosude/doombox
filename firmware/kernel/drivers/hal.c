/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

#include "hal.h"
#include "audio/pz_act.h"

// Pico GPIO pin for the onboard speaker
#define GPIO_OBSPKR 16

bool
hal_init(void)
{
    pz_act_init(GPIO_OBSPKR);

    return true;
}
