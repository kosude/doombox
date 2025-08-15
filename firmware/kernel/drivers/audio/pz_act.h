/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

/******************************************************************************
 * pz_act.h
 * Functions to drive the active piezoelectric buzzer connected directly to the
 * Pico (i.e. the "on-board speaker").
 *****************************************************************************/

#pragma once
#ifndef __audio__pzact_h__
#define __audio__pzact_h__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * Initialise the Pico to drive the on-board speaker on GPIO pin `gpio`.
 */
void
pz_act_init(const uint8_t gpio);

// TODO piezo driver

#ifdef __cplusplus
}
#endif
#endif
