#ifndef BOX2D_3DGBA_H
#define BOX2D_3DGBA_H


#include <cmath>
#include <cstdint>
#include <sstream>
#include <string>

#include <gba.h>

#include "3Dgba/data_types.h"

struct AABBs_2d
{
    int16_t minX;
    int16_t maxX;
    int16_t minY;
    int16_t maxY;

    __attribute__((always_inline)) AABBs_2d() {}
    __attribute__((always_inline)) AABBs_2d(int16_t minX, int16_t maxX, int16_t minY, int16_t maxY, int16_t minZ, int16_t maxZ) :
        minX(minX), maxX(maxX), minY(minY), maxY(maxY) {}

    __attribute__((always_inline)) Vector3i_t getCenter() const {
        return Vector3i_t((maxX + minX) >> 1, (maxY + minY) >> 1);
    }
};

struct AABBi_3d
{
    fix14_t minX;
    fix14_t maxX;
    fix14_t minY;
    fix14_t maxY;

    __attribute__((always_inline)) AABBi_3d() {}
    __attribute__((always_inline)) AABBi_3d(const AABBs_2d &b) :
        minX(b.minX), maxX(b.maxX), minY(b.minY), maxY(b.maxY) {}
    __attribute__((always_inline)) AABBi_3d(fix14_t minX, fix14_t maxX, fix14_t minY, fix14_t maxY, fix14_t minZ, fix14_t maxZ) :
        minX(minX), maxX(maxX), minY(minY), maxY(maxY) {}

    __attribute__((always_inline)) Vector3i_t getCenter() const {
        return Vector3i_t((maxX + minX) >> 1, (maxY + minY) >> 1);
    }
};

inline std::string box_to_string(const AABBi_3d& box) {
    std::stringstream ss;
    ss << "minX: " << box.minX << ", maxX: " << box.maxX << ", "
       << "minY: " << box.minY << ", maxY: " << box.maxY;
    return ss.str();
}

#endif // BOX2D_3DGBA_H