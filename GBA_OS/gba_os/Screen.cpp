#include "gba_os/Screen.h"

namespace gba_os{
namespace screen{

void set_palette_mode_4(const uint16_t* paletteBuffer) {
    // Ensure the palette buffer is not null
    if (paletteBuffer == nullptr) return;

    // Use DMA to transfer the palette data in 32-bit mode
    // The GBA palette memory starts at 0x05000000
    // There are 256 colors, each 2 bytes (16 bits), so 512 bytes in total
    // Since we are using 32-bit mode, we divide the total byte count by 4
    DMA3COPY(paletteBuffer, (void*)BG_PALETTE, DMA_DST_INC | DMA_SRC_INC | DMA32 | DMA_IMMEDIATE | (512 / 4));
}

} // screen
} // gba_os
