#ifndef UI_OVERLAY_H
#define UI_OVERLAY_H

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdio>

void set_font(const uint8_t* font, size_t font_data_length);
void draw_character(void* fb, int x, int y, char c, uint8_t color);
template<typename... Args>
void print_text(void* fb, int x, int y, uint8_t color, const char* s, Args... args){
    char buf[256];
    std::sprintf((char*)buf, s, args...);
    for(int i = 0; i < std::strlen(buf); i++)
        draw_character(fb, x + i * 8, y, buf[i], color);
}

#endif // UI_OVERLAY_H