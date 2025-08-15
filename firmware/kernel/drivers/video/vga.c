/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

#include <hardware/gpio.h>
#include <hardware/spi.h>

#include "vga.h"

// SPI instance reserved for video data transmission
#define SPI spi0

void
vga_init(const struct vga_outputs op)
{
    // initialise chip-select (CS) pin high (deselecting the device)
    gpio_init(op.gpio_scsvga);
    gpio_set_dir(op.gpio_scsvga, GPIO_OUT);
    gpio_put(op.gpio_scsvga, 1);

    // init spi instance
    spi_init(SPI, 10000);
    spi_set_format(SPI, 8, 1, 1, SPI_MSB_FIRST); // TODO document values

    gpio_set_function(op.gpio_scl0, GPIO_FUNC_SPI);
    gpio_set_function(op.gpio_sdo0, GPIO_FUNC_SPI);

    // -----------------------------------------------
    // TODO modularise: the following is testing only!

    // make all pins output
    gpio_put(op.gpio_scsvga, 0);
    spi_write_blocking(SPI, (uint8_t[]){ 0x40 }, 1);
    spi_write_blocking(SPI, (uint8_t[]){ 0x00 }, 1);
    spi_write_blocking(SPI, (uint8_t[]){ 0x00 }, 1);
    gpio_put(op.gpio_scsvga, 1);

    for (;;) {
        // start
        gpio_put(op.gpio_scsvga, 0);

        // A0 and A1 both tied to gnd, so MCP client address is 0100000x
        // where x is 0 for write and 1 for read. (so for writing, 0x40)
        // The GPIO register is on address 0x09.
        spi_write_blocking(SPI, (uint8_t[]){ 0x40 }, 1);
        spi_write_blocking(SPI, (uint8_t[]){ 0x09 }, 1);
        spi_write_blocking(SPI, (uint8_t[]){ 0b11000001 }, 1);

        // stop
        gpio_put(op.gpio_scsvga, 1);
    }
}
