/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

/******************************************************************************
 * pz_act.h
 * Functions to asynchronously drive the active piezoelectric buzzer connected
 * directly to the Pico (i.e. the "on-board speaker").
 *****************************************************************************/

#pragma once
#ifndef __audio__pzact_h__
#define __audio__pzact_h__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

/**
 * Initialise the Pico to drive the on-board speaker on GPIO pin `gpio`.
 */
void
pz_act_init(const uint8_t gpio);

/**
 * Enable the on-board speaker for `ms` milliseconds (non-blocking).
 *
 * Returns true if the speaker successfully enabled, or false if it is busy.
 */
bool
pz_act_for_ms(const uint32_t ms);

/**
 * Beep the on-board speaker `n` times, with it sounding for `ms_on`
 * milliseconds and with `ms_off` delay between beeps.
 *
 * Returns true if the sequence was successfully started, or false if the
 * speaker is busy sounding a sequence so couldn't start this one.
 */
bool
pz_act_repeat(const uint32_t ms_on, const uint32_t ms_off, const uint32_t n);

#ifdef __cplusplus
}
#endif
#endif
