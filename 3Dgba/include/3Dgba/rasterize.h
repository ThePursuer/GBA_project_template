#ifndef RASTERIZE_3DGBA_H
#define RASTERIZE_3DGBA_H

#include <array>

#include "3Dgba/data_types.h"


namespace gba_3D {

struct transformed_face{
    uint16_t next_index = 0;
    Triangle2Ds_t triangle;
    uint8_t color;
};

union vertex_int32_u{
    Vector2s_t v;
    int32_t i;
};

void sort_inplace(vertex_int32_u (&a)[3]);
void drawTriangle(Vector2s_t (&tri)[3], void* fb);
void add_transformed_face(const Triangle2Ds_t& triangle, const uint16_t depth, const uint8_t color);

void rasterize(void* fb);

}
// void rasterize_c(void* fb, uint32_t flags, VertexLink* top);



#endif // RASTERIZE_3DGBA_H