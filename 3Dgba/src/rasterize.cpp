#include "3Dgba/rasterize.h"

namespace gba_3D {

IWRAM_CODE ARM_CODE inline void sort_inplace(Triangle<Vector2<fix14_t>> &arr){
    uint64_t (&a)[3] = *reinterpret_cast<uint64_t(*)[3]>(&arr.verticies);
    uint64_t swap = a[0];
    if (a[0] > a[1]){
        a[0]= a[1];
        a[1]= swap;
    }
    if (a[0] > a[2]){
        swap = a[0];
        a[0] = a[2];
        a[2] = swap;
    }
    if (a[1] > a[2]){
        swap = a[1];
        a[1] = a[2];
        a[2] = swap;
    }
}

static uint16_t internal_color = 0x14;

inline IWRAM_CODE ARM_CODE void drawLines(uint8_t* fb, uint8_t*& starting_line, fix14_t& lx, fix14_t& rx, const fix14_t leftDx, const fix14_t rightDx, int height, uint8_t temp_color){
    // while(starting_line > fb + (240 * 160) && height--){
    //     lx -= leftDx;
    //     rx -= rightDx;
    //     starting_line -= 240;
    // }
    while(height--){ //&& starting_line >= fb){
        uint8_t* dest = starting_line + fix14_to_int(lx);
        int width = fix14_to_int(rx - lx);

        // align to 16 bit boundary
        if (intptr_t(dest) & 1) {
            dest--;
            *(uint16_t*)dest = *dest | (internal_color << 8);
            dest += 2;
            width--;
        }

        switch (width & 3)
        {
        case 3:
            // fallthrough to case 1
        case 1:
            *(uint16_t*)(dest + width - 1) = (temp_color) | (dest[width] << 8);
            width--;
            break;
        case 2:
            *(uint16_t*)dest = internal_color;
            dest+=2;
            width--;
            break;
        default:
            break;
        }

        width >>= 1;
        if (width > 0)
            // memset16(dest, internal_color, width);
            DMA3COPY(&internal_color, (void*)dest, (DMA_IMMEDIATE | DMA16 | DMA_SRC_FIXED | DMA_DST_INC | width));

        lx -= leftDx;
        rx -= rightDx;
        starting_line -= 240;
    }
}

void drawTriangle(uint8_t* fb, Triangle<Vector2<fix14_t>>& tri, uint8_t color)
{
    // int top_vertex = get_top_vertex(tri);
    tri.sort();
    Vector2<fix14_t>& a = tri.verticies[0];
    Vector2<fix14_t>& b = tri.verticies[1];
    Vector2<fix14_t>& c = tri.verticies[2];

    /* here we know that tri[0].y <= tri[1].y <= tri[2].y */
    /* aka vertex A <= B <= C */
    fix14_t CAh = 0, CBh = 0;
    CAh = c.y - a.y;
    CBh = c.y - b.y;

    fix14_t CAdx = fix14_div(c.x - a.x, CAh);
    fix14_t CBdx = fix14_div(c.x - b.x, CBh);
    fix14_t BAdx = fix14_div(b.x - a.x, b.y - a.y);

    uint8_t* line_ptr = fb + (240 * fix14_to_int(c.y));
    internal_color = color | color << 8;

    fix14_t lx, rx;
    if(CBh == 0){
        lx = b.x;
        rx = c.x;
        drawLines(fb, line_ptr, lx, rx, BAdx, CAdx, fix14_to_int(CAh - CBh), color);
        return;
    }
    else {
        lx = c.x;
        rx = c.x;
    }
    if (CBdx > CAdx){
        // weierd case
        drawLines(fb, line_ptr, lx, rx, CBdx, CAdx, fix14_to_int(CBh), color);
        drawLines(fb, line_ptr, lx, rx, BAdx, CAdx, fix14_to_int(CAh - CBh), color);
    }
    else {
        drawLines(fb, line_ptr, lx, rx, CAdx, CBdx, fix14_to_int(CBh), color);
        drawLines(fb, line_ptr, lx, rx, CAdx, BAdx, fix14_to_int(CAh - CBh), color);
    }

}

}

