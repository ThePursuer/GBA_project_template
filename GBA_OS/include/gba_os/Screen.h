#ifndef GBA_SCREEN_H
#define GBA_SCREEN_H

#include <gba_dma.h>
#include <gba_video.h>

namespace Gba_os{
namespace screen{

void set_user_mode(int mode); // sets the screen mode for the user
void restore_user_mode(); // restores the screen mode to the user mode
void set_palette_mode_4(const uint16_t* paletteBuffer);

} // screen
} // gba_os

#endif // GBA_SCREEN_H