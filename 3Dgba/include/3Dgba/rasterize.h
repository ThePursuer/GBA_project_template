#ifndef RASTERIZE_3DGBA_H
#define RASTERIZE_3DGBA_H

#include <array>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include "3Dgba/data_types.h"
#include "3Dgba/fast.h"

#include "gba.h"


namespace gba_3D {

void drawTriangle(uint8_t* fb, Triangle<Vector2<fix14_t>>& tri, uint8_t color);

void add_transformed_face(const Triangle<Vector2<fix14_t>>& triangle, const uint16_t depth, const uint8_t color);
void rasterize(void* fb);

} // namespace gba_3D

#endif // RASTERIZE_3DGBA_H