/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

#include <hardware/clocks.h>
#include <hardware/pwm.h>
#include <hardware/sync.h>
#include <pico/stdlib.h>

// GPIO pin for the onboard speaker
#define GPIO_OBSPKR 22

// GPIO pins for left and right PWM audio, respectively
#define GPIO_PWM_AUL 8
#define GPIO_PWM_AUR 9

// see 12.5.2: https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf
#define SLICE_AUX 4 // GPIO8 and GPIO9
#define CHAN_AUL  0 // GPIO8
#define CHAN_AUR  1 // GPIO9

#include "pwm_samples.h"
static int cursor = 0;

// PWM interrupt handler which advances the sample cursor.
// Bit-shifting by 3 means each sample is played for 8 cycles
void
pwm_isr()
{
    pwm_clear_irq(SLICE_AUX);

    if (cursor < (samplen << 3) - 1) {
        // not doing stereo stuff yet
        pwm_set_chan_level(SLICE_AUX, CHAN_AUL, samples[cursor >> 3]);
        pwm_set_chan_level(SLICE_AUX, CHAN_AUR, samples[cursor >> 3]);
        cursor++;
    } else {
        // reset to start
        cursor = 0;
    }
}

int
main(void)
{
    // GPIO init
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_init(GPIO_OBSPKR);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_set_dir(GPIO_OBSPKR, GPIO_OUT);

    // turn on onboard LED
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    gpio_put(PICO_DEFAULT_LED_PIN, 0);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);

    // TODO optionally beep onboard speaker on startup
    // gpio_put(GPIO_OBSPKR, 1);
    // sleep_ms(100);
    // gpio_put(GPIO_OBSPKR, 0);

    gpio_set_function(GPIO_PWM_AUL, GPIO_FUNC_PWM);
    gpio_set_function(GPIO_PWM_AUR, GPIO_FUNC_PWM);

    pwm_config cfg = pwm_get_default_config();

    // ------------------------------------------------------------------------

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

    pwm_config_set_clkdiv_mode(&cfg, PWM_DIV_FREE_RUNNING);
    pwm_config_set_phase_correct(&cfg, 0);
    pwm_config_set_clkdiv_int_frac(&cfg, 0, 0);
    pwm_config_set_wrap(&cfg, 1119);

    pwm_init(SLICE_AUX, &cfg, true);

    pwm_set_chan_level(SLICE_AUX, CHAN_AUL, 100);
    pwm_set_chan_level(SLICE_AUX, CHAN_AUR, 100);

    // ------------------------------------------------------------------------

    // // Playing WAV audio test
    // // ----------------------------
    // // system freq 150 MHz (correct?)
    // // with TOP = 213
    // //      CSR_PH_CORRECT = 0
    // // playing each sample for 8 cycles, so for a sample rate of...
    // //      11kHz --> DIV = 8.0
    // //      22kHz --> DIV = 4.0, etc.

    // // setup interrupt handler
    // pwm_clear_irq(SLICE_AUX);
    // pwm_set_irq_enabled(SLICE_AUX, 1);
    // irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_isr);
    // irq_set_enabled(PWM_IRQ_WRAP, 1);

    // pwm_config_set_clkdiv_mode(&cfg, PWM_DIV_FREE_RUNNING);
    // pwm_config_set_phase_correct(&cfg, 0);
    // pwm_config_set_clkdiv(&cfg, 8.0);
    // pwm_config_set_wrap(&cfg, 213);

    // pwm_init(SLICE_AUX, &cfg, true);

    // // hold for interrupts
    // for (;;) {
    //     __wfi(); // ARM instruction
    // }

    // ------------------------------------------------------------------------
}
