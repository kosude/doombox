/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

#include <hardware/gpio.h>
#include <pico/time.h>

#include "pz_act.h"

// On-board active speaker GPIO pin (set via pz_act_init)
static volatile uint8_t _obspkr_gpio;

// True when the on-board active speaker is enabled.
static volatile bool _obspkr_on;

// Repeating timer used for repeatedly beeping the obspkr.
static struct repeating_timer _rep_timer;
// True when the repeating timer is busy (i.e. not cancelled).
static volatile bool _rep_timer_busy;
// Counter variables (n=total, i=cumulative) for repeating beeps
static volatile uint32_t _rep_timer_n, _rep_timer_i;

// Alarm interrupt handler to disable the speaker after a delay
static int64_t
_alarm_disable_pz(alarm_id_t id, void *data)
{
    gpio_put(_obspkr_gpio, 0);
    _obspkr_on = false;
    return 0;
}

// Repeating timer interrupt handler to beep the on-board speaker for a
// duration.
static bool
_retimer_beep_pz(struct repeating_timer *timer)
{
    if (++_rep_timer_i >= _rep_timer_n) {
        cancel_repeating_timer(timer);
        _rep_timer_busy = false;
    }

    pz_act_for_ms((uint32_t)timer->user_data);

    return true;
}

void
pz_act_init(const uint8_t gpio)
{
    gpio_init(gpio);
    gpio_set_dir(gpio, GPIO_OUT);

    _obspkr_gpio = gpio;
}

bool
pz_act_for_ms(const uint32_t ms)
{
    if (_obspkr_on) {
        return false;
    }
    _obspkr_on = true;

    gpio_put(_obspkr_gpio, 1);
    add_alarm_in_ms(ms, _alarm_disable_pz, NULL, true);

    return true;
}

bool
pz_act_repeat(const uint32_t ms_on, const uint32_t ms_off, const uint32_t n)
{
    if (_rep_timer_busy) {
        return false;
    }
    _rep_timer_busy = true;
    _rep_timer_n = n;
    _rep_timer_i = 0;

    add_repeating_timer_ms(ms_off + ms_on,
                           _retimer_beep_pz,
                           (void *)ms_on,
                           &_rep_timer);

    return true;
}
