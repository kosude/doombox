/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

#include <hardware/dma.h>
#include <hardware/pio.h>
#include <string.h>

#include "vga.h"
#include "vram.h"

// PIO instance used for driving VGA signals
#define pio      pio0
#define pio_irq0 PIO0_IRQ_0

#define DMA_CHAN_VIDEO  0
#define DMA_CHAN_RECONF 1

uint8_t __vram_start[VRAM_LEN]; // array contains pixel data for both buffers
uint8_t *__vram_back = &__vram_start[VRAM_FB_LEN]; // back buffer

static volatile uint8_t *_vram_ptr = &__vram_start[0];
static volatile size_t _scanline = 0;
static volatile uint8_t _frame = 0; // frame = 0 or 1, determines drawn buffer

static volatile bool _blanking = false;

// Called at the end of each memory transfer of a line in the vram.
void
__not_in_flash_func(_dma_adv_dram_ptr)()
{
    // clear the interrupt request
    dma_hw->ints0 = 1u << DMA_CHAN_VIDEO;

    // increment ptr line (between 0-399)
    _scanline++;
    if (_scanline >= VGA_SCAN_HEIGHT) {
        _scanline = 0;
    }

    // pointer indexed to each line twice
    _vram_ptr = &__vram_start[VRAM_FB_LEN * _frame
                              + VGA_WIDTH * ((_scanline + 1) >> 1)];
}

// Called at the start of each video blanking region.
void
__not_in_flash_func(_pio_blanking_swap)()
{
    pio_interrupt_clear(pio, 2);
    _blanking = true; // signals vram_swap() to swap pointers
}

uint32_t
vram_dma_channel_configure(const uint8_t sm)
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
                          &_vram_ptr,
                          1,
                          false);

    // IRQ (_dma_adv_dram_ptr) called each time the video DMA channel ends,
    // which updates _vram_ptr accordingly, before the reconf channel passes
    // it to the video channel's next iteration.
    dma_channel_set_irq0_enabled(DMA_CHAN_VIDEO, true);
    irq_set_exclusive_handler(DMA_IRQ_0, _dma_adv_dram_ptr);
    irq_set_enabled(DMA_IRQ_0, true);
    irq_set_priority(DMA_IRQ_0, 0); // time-critical; use highest priority

    pio_set_irq0_source_enabled(pio, pis_interrupt2, true);
    irq_set_exclusive_handler(pio_irq0, _pio_blanking_swap);
    irq_set_enabled(pio_irq0, true);

    // chan0 must be started manually
    return DMA_CHAN_VIDEO;
}

void
vram_swap(void)
{
    // wait for next blanking region
    while (!_blanking) {
    }
    _blanking = false;

    // swap pointers (_frame is used in the dma irq to index through front buf)
    _frame = !_frame;
    __vram_back = &__vram_start[VRAM_FB_LEN * !_frame];
}

void
vram_clear(const uint8_t col)
{
    // clear the back buffer
    memset(__vram_back, col, VRAM_FB_LEN);
}
