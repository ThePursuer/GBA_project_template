#include "gba_os/Screen.h"

#include "OsErrors.h"

namespace Gba_os{
namespace screen{

EWRAM_DATA static int user_mode;

void set_user_mode(int mode) {
    user_mode = mode;
    SetMode(mode);
}

void restore_user_mode() {
    SetMode(user_mode);
}

void set_palette_mode_4(const uint16_t* paletteBuffer) {
    // Ensure the pointer is not null
    if (paletteBuffer == nullptr)
        Gba_os::error::error_state(Gba_os::error::INVALID_PALLET, (void*)paletteBuffer);
    DMA3COPY(paletteBuffer, BG_PALETTE, DMA_DST_INC | DMA_SRC_INC | DMA32 | DMA_IMMEDIATE | (256 / 2));
}

} // screen
} // gba_os
