/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

/******************************************************************************
 * panic.h
 * Kernel panic functions.
 *****************************************************************************/

#pragma once
#ifndef __kernel__panic_h__
#define __kernel__panic_h__
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Enter a kernel panic. Hangs the microcontroller until reset.
 */
void
kpanic(void);

#ifdef __cplusplus
}
#endif
#endif
