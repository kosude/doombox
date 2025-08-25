/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

#include <hardware/dma.h>
#include <pico/stdlib.h>
#include <string.h>

#include "vga.h"
#include "vram.h"

#define DMA_CHAN_VIDEO  0
#define DMA_CHAN_RECONF 1

uint8_t __vram_start[VRAM_LEN]; // array contains pixel data for both buffers
uint8_t *__vram_back = &__vram_start[VRAM_FB_LEN]; // back buffer

static volatile uint8_t *__vram_ptr = &__vram_start[0];
static volatile size_t __scanline = 0;
static volatile uint8_t __frame = 0; // frame = 0 or 1, determines drawn buffer

void
__not_in_flash_func(_dma_adv_dram_ptr)()
{
    // clear the interrupt request
    dma_hw->ints0 = 1u << DMA_CHAN_VIDEO;

    // increment ptr line (between 0-399)
    __scanline++;
    if (__scanline >= VGA_SCAN_HEIGHT) {
        __scanline = 0;

        // copy back buffer to front buffer
        memcpy(__vram_start, __vram_back, VRAM_FB_LEN);
        __frame = !__frame;
    }

    // pointer indexed to each line twice
    __vram_ptr = &__vram_start[VRAM_FB_LEN * __frame
                               + VGA_WIDTH * ((__scanline + 1) >> 1)];
}

uint32_t
vram_dma_channel_configure(pio_hw_t *const pio, const uint8_t sm)
{
    dma_channel_config cfg;

    // initialise video DMA channel (pushing pixel data to PIO video machine)
    cfg = dma_channel_get_default_config(DMA_CHAN_VIDEO);
    channel_config_set_transfer_data_size(&cfg, DMA_SIZE_8);
    channel_config_set_read_increment(&cfg, true);
    channel_config_set_write_increment(&cfg, false);
    channel_config_set_dreq(&cfg, DREQ_PIO0_TX2);
    channel_config_set_chain_to(&cfg, DMA_CHAN_RECONF);
    dma_channel_configure(DMA_CHAN_VIDEO,
                          &cfg,
                          &pio->txf[sm],
                          &__vram_start,
                          VGA_WIDTH,
                          false);

    // initialise control DMA channel (reconfigures + resets the video channel)
    cfg = dma_channel_get_default_config(DMA_CHAN_RECONF);
    channel_config_set_transfer_data_size(&cfg, DMA_SIZE_32);
    channel_config_set_read_increment(&cfg, false);
    channel_config_set_write_increment(&cfg, false);
    channel_config_set_chain_to(&cfg, DMA_CHAN_VIDEO);
    dma_channel_configure(DMA_CHAN_RECONF,
                          &cfg,
                          &dma_hw->ch[DMA_CHAN_VIDEO].read_addr,
                          &__vram_ptr,
                          1,
                          false);

    // IRQ (_dma_adv_dram_ptr) called each time the video DMA channel ends,
    // which updates __vram_ptr accordingly, before the reconf channel passes
    // it to the video channel's next iteration.
    dma_channel_set_irq0_enabled(DMA_CHAN_VIDEO, true);
    irq_set_exclusive_handler(DMA_IRQ_0, _dma_adv_dram_ptr);
    irq_set_enabled(DMA_IRQ_0, true);
    irq_set_priority(DMA_IRQ_0, 0); // time-critical; use highest priority

    // chan0 must be started manually
    return DMA_CHAN_VIDEO;
}

void
vram_clear(const uint8_t col)
{
    memset(__vram_start, col, VRAM_LEN);
}
