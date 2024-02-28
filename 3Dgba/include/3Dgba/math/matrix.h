#ifndef MATRIX_3DGBA_H
#define MATRIX_3DGBA_H

#include <cmath>
#include <cstdint>
#include <string>
#include <iostream>
#include <sstream>
#include <array>

#include <gba.h>

#include "3Dgba/data_types.h"
#include "3Dgba/math/fix14.h"
#include "3Dgba/math/fix7.h"
#include "3Dgba/math/math.h"



#define ANGLE_360       0x10000
#define ANGLE_0         0
#define ANGLE_1         (ANGLE_360 / 360)
#define ANGLE_45        (ANGLE_360 / 8)      // != 45 * ANGLE_1 !!!
#define ANGLE_90        (ANGLE_360 / 4)      // != 90 * ANGLE_1 !!!
#define ANGLE_180      -(ANGLE_360 / 2)      // INT16_MIN
#define ANGLE(x)        ((x) * ANGLE_1)
#define ANGLE_SHIFT_45  13
#define ANGLE_SHIFT_90  14
#define ANGLE_SHIFT_180 15
#define FIXED_SHIFT     14

struct Matrix
{
    fix14_t e00, e01, e02, e03,
            e10, e11, e12, e13,
            e20, e21, e22, e23;
};

Matrix& get_matrix_ptr();

inline bool operator==(const Matrix& lhs, const Matrix& rhs)
{
    return (lhs.e00 == rhs.e00 && lhs.e01 == rhs.e01 && lhs.e02 == rhs.e02 && lhs.e03 == rhs.e03 &&
           lhs.e10 == rhs.e10 && lhs.e11 == rhs.e11 && lhs.e12 == rhs.e12 && lhs.e13 == rhs.e13 &&
           lhs.e20 == rhs.e20 && lhs.e21 == rhs.e21 && lhs.e22 == rhs.e22 && lhs.e23 == rhs.e23);
}

#define ENCODE_ANGLES(x, y, z) \
    (((x >> 2) & 0x3FF0) << 16) | (((y >> 12) & 0x000F) << 16) | (((y << 4) & 0xFC00) | ((z >> 6) & 0x03FF))

// Dot product of two 4D vectors (ax, ay, az, aw) and (bx, by, bz)
#define DP43(ax,ay,az,aw,bx,by,bz)  (ax * bx + ay * by + az * bz + aw)

// Dot product of two 3D vectors (ax, ay, az) and (bx, by, bz)
#define DP33(ax,ay,az,bx,by,bz)     (ax * bx + ay * by + az * bz)

void matrixLinearInterpolation(Matrix& a, const Matrix& b, fix14_t weight);

#ifdef __cplusplus
extern "C" {
#endif
// Function declarations
void matrixLerp_c(const Matrix &n, Matrix&m, int32_t pmul, int32_t pdiv);
void matrixTranslateRel_c(Matrix &m, int32_t x, int32_t y, int32_t z);
void matrixTranslateAbs_c(Matrix& m, Vector3<fix14_t>& cameraPos, int32_t x, int32_t y, int32_t z);
void matrixTranslateSet_c(Matrix &m, int32_t x, int32_t y, int32_t z);
void matrixRotateX_c(Matrix &m, int32_t angle);
void matrixRotateY_c(Matrix &m, int32_t angle);
void matrixRotateZ_c(Matrix &m, int32_t angle);
void matrixRotateYQ_c(Matrix &m, int32_t quadrant);
void matrixRotateYXZ_c(Matrix &m, int32_t angleX, int32_t angleY, int32_t angleZ);

inline void matrixSetBasis_c(Matrix &dst, const Matrix &src)
{
    dst.e00 = src.e00;
    dst.e01 = src.e01;
    dst.e02 = src.e02;

    dst.e10 = src.e10;
    dst.e11 = src.e11;
    dst.e12 = src.e12;

    dst.e20 = src.e20;
    dst.e21 = src.e21;
    dst.e22 = src.e22;
}

inline void matrixSetIdentity_c(Matrix &m) {
    m.e00 = 0x4000;
    m.e01 = 0;
    m.e02 = 0;
    m.e03 = 0;

    m.e10 = 0;
    m.e11 = 0x4000;
    m.e12 = 0;
    m.e13 = 0;

    m.e20 = 0;
    m.e21 = 0;
    m.e22 = 0x4000;
    m.e23 = 0;
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif // MATRIX_3DGBA_H
