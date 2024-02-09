#ifndef MY_HEADER_FILE_H
#define MY_HEADER_FILE_H

#include <cmath>
#include <cstdint>
#include <sstream>
#include <string>

#include <gba.h>

#include "3Dgba/data_types.h"

struct AABBs
{
    int16_t minX;
    int16_t maxX;
    int16_t minY;
    int16_t maxY;
    int16_t minZ;
    int16_t maxZ;

    __attribute__((always_inline)) AABBs() {}
    __attribute__((always_inline)) AABBs(int16_t minX, int16_t maxX, int16_t minY, int16_t maxY, int16_t minZ, int16_t maxZ) :
        minX(minX), maxX(maxX), minY(minY), maxY(maxY), minZ(minZ), maxZ(maxZ) {}

    __attribute__((always_inline)) Vector3i_t getCenter() const {
        return Vector3i_t((maxX + minX) >> 1, (maxY + minY) >> 1, (maxZ + minZ) >> 1);
    }
};

struct AABBi
{
    int32_t minX;
    int32_t maxX;
    int32_t minY;
    int32_t maxY;
    int32_t minZ;
    int32_t maxZ;

    __attribute__((always_inline)) AABBi() {}
    __attribute__((always_inline)) AABBi(const AABBs &b) :
        minX(b.minX), maxX(b.maxX), minY(b.minY), maxY(b.maxY), minZ(b.minZ), maxZ(b.maxZ) {}
    __attribute__((always_inline)) AABBi(int32_t minX, int32_t maxX, int32_t minY, int32_t maxY, int32_t minZ, int32_t maxZ) :
        minX(minX), maxX(maxX), minY(minY), maxY(maxY), minZ(minZ), maxZ(maxZ) {}

    __attribute__((always_inline)) Vector3i_t getCenter() const {
        return Vector3i_t((maxX + minX) >> 1, (maxY + minY) >> 1, (maxZ + minZ) >> 1);
    }
};

inline std::string box_to_string(const AABBi& box) {
    std::stringstream ss;
    ss << "minX: " << box.minX << ", maxX: " << box.maxX << ", "
       << "minY: " << box.minY << ", maxY: " << box.maxY << ", "
       << "minZ: " << box.minZ << ", maxZ: " << box.maxZ;
    return ss.str();
}

void boxRotateYQ_c(AABBi &box, int32_t quadrant);
extern "C" void boxTranslate_asm(AABBi &box, int32_t x, int32_t y, int32_t z);

#endif // MY_HEADER_FILE_H