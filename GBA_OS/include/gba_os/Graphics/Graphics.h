#ifndef GBA_GRAPHICS_H
#define GBA_GRAPHICS_H

#include <climits>
#include <array>
#include <algorithm>

#include <gba_video.h>
#include <gba_dma.h>

#include "gba_os/CoreDefs.h"
#include "gba_os/Graphics/Types.h"

namespace gba_os::graphics {

bool LoadOBJFile(const uint8_t* data, size_t length, Mesh& mesh);
bool LoadMTLFile256Colors(const uint8_t* data, size_t length, Material& material);
uint16_t MatchColorTo256ColorPalette(const RGB& color);
void set_palette_mode_4(const uint16_t* paletteBuffer); // sets BG_PALLET and then sorts the colors in ascending order

}

namespace gba_os::tasks {

void update_screen_mode_4_task(Task& task);

} // gba_os


#endif // GBA_GRAPHICS_H