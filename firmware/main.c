/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

#include <hardware/clocks.h>
#include <hardware/pwm.h>
#include <pico/stdlib.h>

// GPIO pins for left and right PWM audio, respectively
#define PWM_AUL 8
#define PWM_AUR 9

// see 12.5.2: https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf
#define SLICE_AUX 4 // GPIO8 and GPIO9
#define CHAN_AUL  0 // GPIO8
#define CHAN_AUR  1 // GPIO9

int
main(void)
{
    // turn on onboard LED
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);

    gpio_set_function(PWM_AUL, GPIO_FUNC_PWM);
    gpio_set_function(PWM_AUR, GPIO_FUNC_PWM);

    // e.g. for outputting a C note
    // ref https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf
    //     sect 12.5.2.6: configuring PWM period
    // ----------------------------
    // target freq = 523.25 Hz
    // and system freq assumed to be 150 MHz (correct?)
    // therefore target period = ~286670 clk cycles per PWM cycle
    // so...
    // with DIV = 256 (max)
    //      CSR_PH_CORRECT = 0 (i.e. not phase-correct mode)
    // working back from formula given in the datasheet:
    //      TOP = ~ 1119
    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_clkdiv_mode(&cfg, PWM_DIV_FREE_RUNNING);
    pwm_config_set_phase_correct(&cfg, 0);
    pwm_config_set_clkdiv_int_frac(&cfg, 0, 0);
    pwm_config_set_wrap(&cfg, 1119); // this sets TOP register

    pwm_init(SLICE_AUX, &cfg, true);

    pwm_set_chan_level(SLICE_AUX, CHAN_AUL, 100);
    pwm_set_chan_level(SLICE_AUX, CHAN_AUR, 100);
}
