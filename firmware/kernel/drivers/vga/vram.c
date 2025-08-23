/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

#include <hardware/dma.h>
#include <string.h>

#include "vga.h"
#include "vram.h"

uint8_t __vram_start[VRAM_LEN];

// stored address to start of vram
const static uint8_t *_VRAM_PTR = __vram_start;

uint32_t
vram_dma_channel_configure(pio_hw_t *const pio, const uint8_t sm)
{
    // channel 0: sends colour data to the PIO video state machine
    // channel 1: resets channel 0 when a full framebuffer has been transferred
    const uint32_t chan0 = 0;
    const uint32_t chan1 = 1;

    // channel 0 configuration
    dma_channel_config c0 = dma_channel_get_default_config(chan0);
    channel_config_set_transfer_data_size(&c0, DMA_SIZE_8); // 8-bit txfers
    channel_config_set_read_increment(&c0, true);   // incr read address
    channel_config_set_write_increment(&c0, false); // don't incr write address
    channel_config_set_dreq(&c0, DREQ_PIO0_TX2);    // TX FIFO for sm 2 (video)
    channel_config_set_chain_to(&c0, chan1);        // trigger channel 1
    dma_channel_configure(chan0,
                          &c0,
                          &pio->txf[sm], // write to TX FIFO for video sm
                          &VRAM,         // read bytes from VRAM
                          64000,
                          false // don't start immediately
    );

    // channel 1 configuration
    dma_channel_config c1 = dma_channel_get_default_config(chan1);
    channel_config_set_transfer_data_size(&c1, DMA_SIZE_32); // 32-bit txfers
    channel_config_set_read_increment(&c1, false);  // don't incr read address
    channel_config_set_write_increment(&c1, false); // don't incr write address
    channel_config_set_chain_to(&c1, chan0);        // trigger channel 0
    dma_channel_configure(chan1,
                          &c1,
                          &dma_hw->ch[chan0].read_addr, // Write address
                                                        // (channel 0 read
                                                        // address)
                          &_VRAM_PTR, // reset chan 0 read address to VRAM
                          1,          // single transfer
                          false       // don't start immediately
    );

    // chan0 must be started manually
    return chan0;
}

// TODO temp function
void
vram_init_palette()
{
    size_t posx = 0, posy = 0;

    // loop through blocks (incr red)
    for (uint8_t r = 0; r < 8; r++) {
        // loop through rows (incr green)
        for (uint8_t g = 0; g < 8; g++) {
            // loop through columns (inc blue)
            for (uint8_t b = 0; b < 4; b++) {
                // encode rgb value (BBGGGRRR)
                uint8_t rgb = (b << 6) | (g << 3) | r;

                // draw a 20x10 block of colour rgb
                for (size_t i = 0; i < 10; i++) {
                    for (size_t j = 0; j < 20; j++) {
                        ((uint8_t *)
                             VRAM)[((posy + i) * VGA_WIDTH) + (posx + j)] =
                            rgb;
                    }
                }

                posx += 20;
            }
            posx -= 80;
            posy += 10;
        }

        if (r != 3) {
            posx += 80;
            posy -= 80;
        } else {
            posx = 0;
        }
    }
}
