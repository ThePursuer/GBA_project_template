#include "gba_os/Screen.h"

#include "OsErrors.h"

namespace gba_os{
namespace screen{


void set_palette_mode_4(const uint16_t* paletteBuffer) {
    // Ensure the pointer is not null
    if (paletteBuffer == nullptr)
        gba_os::error::error_state(gba_os::error::INVALID_PALLET, (void*)paletteBuffer);
    DMA3COPY(paletteBuffer, BG_PALETTE, DMA_DST_INC | DMA_SRC_INC | DMA32 | DMA_IMMEDIATE | (256 / 2));
}

} // screen
} // gba_os
