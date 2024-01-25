#ifndef GBA_SCREEN_H
#define GBA_SCREEN_H

#include <gba_dma.h>
#include <gba_video.h>

namespace gba_os{
namespace screen{

static inline void set_mode4(int mode = 0){ SetMode(MODE_4 | BG2_ENABLE | mode);}
void set_palette_mode_4(const uint16_t* paletteBuffer);

} // screen
} // gba_os

#endif // GBA_SCREEN_H