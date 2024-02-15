#include "3Dgba/ui_overlay.h"

static const uint8_t* font_;
static size_t font_data_length_;

void set_font(const uint8_t* font, size_t font_data_length){
    font_ = font;
    font_data_length_ = font_data_length;
}

void draw_character(void* fb, int x, int y, char c, uint8_t color){
    const uint8_t* char_data = font_ + (c * 8); // Calculate the correct offset for each character
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (char_data[i] & (1 << (7 - j))) // Reverse the order of the bits in each byte
                *((uint8_t*)fb + ((y + i) * 240 + (x + j))) = color;
        }
    }
}
