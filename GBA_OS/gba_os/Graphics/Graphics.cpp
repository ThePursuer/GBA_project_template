#include "gba_os/Graphics/Graphics.h"

namespace gba_os::graphics {

bool LoadOBJFile(const uint8_t* data, size_t length, Mesh& mesh) {
    // Convert the binary data to a string
    std::string objData(reinterpret_cast<const char*>(data), length);

    std::istringstream stream(objData);
    std::string line;

    while (std::getline(stream, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            float x, y, z;
            iss >> x >> y >> z;
            Vector3 v = { fix16_from_float(x), fix16_from_float(y), fix16_from_float(z) };
            mesh.vertices.push_back(v);
        } else if (prefix == "vn") {
            float nx, ny, nz;
            iss >> nx >> ny >> nz;
            Vector3 n = { fix16_from_float(nx), fix16_from_float(ny), fix16_from_float(nz) };
            mesh.normals.push_back(n);
        } else if (prefix == "f") {
            Face f;
            char slash;
            for (int i = 0; i < 3; ++i) { // Assuming triangle faces
                iss >> f.v[i] >> slash >> slash >> f.vn[i];
                f.v[i]--; // OBJ indices are 1-based
                f.vn[i]--;
            }
            mesh.faces.push_back(f);
        }
    }

    return true;
}

uint16_t MatchColorTo256ColorPalette(const RGB& color) {
    int left = 0;
    int right = 255;
    int closestIndex = -1;
    int closestDistance = INT_MAX;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        // Assuming BG_PALETTE is an array of RGB555 formatted colors
        int paletteColor = BG_PALETTE[mid];
        int pr = (paletteColor & 0x7C00) >> 10;
        int pg = (paletteColor & 0x03E0) >> 5;
        int pb = paletteColor & 0x001F;

        int dr = color.r - (pr << 3);  // Convert RGB555 to RGB888 for comparison
        int dg = color.g - (pg << 3);
        int db = color.b - (pb << 3);
        int distance = dr * dr + dg * dg + db * db;

        if (distance < closestDistance) {
            closestDistance = distance;
            closestIndex = mid;
        }

        if (distance == 0) {
            // Exact match found
            return (uint16_t)mid;
        }

        // Adjust search range
        if (color.r < (pr << 3)) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    return (uint16_t)closestIndex;
}

bool LoadMTLFile256Colors(const uint8_t* data, size_t length, Material& material) {
    std::string mtlData(reinterpret_cast<const char*>(data), length);
    std::istringstream stream(mtlData);
    std::string line;

    while (std::getline(stream, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "Kd") {
            float r, g, b;
            iss >> r >> g >> b;
            material.originalColor = { static_cast<uint8_t>(r * 255), static_cast<uint8_t>(g * 255), static_cast<uint8_t>(b * 255) };
            material.colorCode = MatchColorTo256ColorPalette(material.originalColor);
        } else if (prefix == "Ns") {
            float Ns;
            iss >> Ns;
            material.Ns = fix16_from_float(Ns);
        } else if (prefix == "Ni") {
            float Ni;
            iss >> Ni;
            material.Ni = fix16_from_float(Ni);
        } else if (prefix == "d") {
            float d;
            iss >> d;
            material.d = fix16_from_float(d);
        } else if (prefix == "illum") {
            iss >> material.illum;
        }
    }

    return true;
}

void set_palette_mode_4(const uint16_t* paletteBuffer) {
    // Ensure the palette buffer is not null
    if (paletteBuffer == nullptr) return;

    // Use DMA to transfer the palette data in 32-bit mode
    // The GBA palette memory starts at 0x05000000
    // There are 256 colors, each 2 bytes (16 bits), so 512 bytes in total
    // Since we are using 32-bit mode, we divide the total byte count by 4
    DMA3COPY(paletteBuffer, (void*)BG_PALETTE, DMA_DST_INC | DMA_SRC_INC | DMA32 | DMA_IMMEDIATE | (512 / 4));

    // Sort the palette
    std::sort(BG_PALETTE, BG_PALETTE + 256);
}

} // gba_os::graphics


namespace gba_os::tasks{

IWRAM_CODE void update_screen_mode_4_task(Task& task){
    void* screen_buffer = task.data;
    static int mode45_active_screen_buffer = 0; 
    // Set the display control and copy the screen data
    switch (mode45_active_screen_buffer) {
        case 0:
            DMA3COPY(screen_buffer, (void*)(VRAM), DMA_DST_INC | DMA_SRC_INC | DMA32 | DMA_IMMEDIATE | (240 * 160 / 4));
            REG_DISPCNT &= ~((u16)BACKBUFFER); // Use front buffer
            break;
        case 1:
            DMA3COPY(screen_buffer, (void*)(VRAM + 0xA000), DMA_DST_INC | DMA_SRC_INC | DMA32 | DMA_IMMEDIATE | (240 * 160 / 4));
            REG_DISPCNT |= (u16)BACKBUFFER;  // Use back buffer
            break;
    }
    mode45_active_screen_buffer = (mode45_active_screen_buffer + 1) % 2;
}

} // gba_os