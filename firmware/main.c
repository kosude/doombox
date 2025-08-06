/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

#include <pico/stdlib.h>
#include <stdio.h>

int
main(void)
{
    // setup LED GPIO
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    for (;;) {
        gpio_put(PICO_DEFAULT_LED_PIN, true);
        sleep_ms(1000);
        gpio_put(PICO_DEFAULT_LED_PIN, false);
        sleep_ms(1000);
    }
}
