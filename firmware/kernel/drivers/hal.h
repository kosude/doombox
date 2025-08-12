/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

/******************************************************************************
 * hal.h
 * Hardware Abstraction Layer (HAL) init and configuration procedures
 *****************************************************************************/

#pragma once
#ifndef __drivers__hal_h__
#define __drivers__hal_h__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

/**
 * Initialise the Hardware Abstraction Layer.
 */
bool
hal_init(void);

#ifdef __cplusplus
}
#endif
#endif
