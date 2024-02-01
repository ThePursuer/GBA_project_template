#include "3Dgba/matrix.h"

#include <gba.h>

IWRAM_DATA vector3i_t gCameraViewPos;

#define sincos(x,s,c) {\
    uint32_t sc = gSinCosTable[uint32_t(x << 16) >> 20];\
    s = int32_t(sc) >> 16;\
    c = int32_t(sc) << 16 >> 16;\
}

#define X_ROTX(x,y,s,c)  (((x) * (c) - (y) * (s)) >> FIXED_SHIFT)
#define X_ROTY(x,y,s,c)  (((y) * (c) + (x) * (s)) >> FIXED_SHIFT)
#define X_ROTXY(x,y,s,c) {\
    x = X_ROTX(x,y,s,c);\
    y = X_ROTY(x,y,s,c);\
}

#define FixedInvS(x)     ((x < 0) ? -divTable[abs(x)] : divTable[x])
#define FixedInvU(x)     divTable[x]

#define LERP_1_2(a, b)   a = (b + a) >> 1
#define LERP_1_3(a, b)   a = a + (b - a) / 3
#define LERP_2_3(a, b)   a = b - (b - a) / 3
#define LERP_1_4(a, b)   a = a + ((b - a) >> 2)
#define LERP_3_4(a, b)   a = b - ((b - a) >> 2)
#define LERP_1_5(a, b)   a = a + (b - a) / 5
#define LERP_2_5(a, b)   a = a + ((b - a) << 1) / 5
#define LERP_3_5(a, b)   a = b - ((b - a) << 1) / 5
#define LERP_4_5(a, b)   a = b - (b - a) / 5
#define LERP_SLOW(a, b)  a = a + ((b - a) * t >> 8)

#define LERP_ROW(lerp_func, a, b, row) \
    lerp_func(a.e##row##0, b.e##row##0); \
    lerp_func(a.e##row##1, b.e##row##1); \
    lerp_func(a.e##row##2, b.e##row##2); \
    lerp_func(a.e##row##3, b.e##row##3);

#define LERP_MATRIX(lerp_func) \
    LERP_ROW(lerp_func, m, n, 0); \
    LERP_ROW(lerp_func, m, n, 1); \
    LERP_ROW(lerp_func, m, n, 2);

void matrixLerp_c(const Matrix &n, Matrix &m, int32_t pmul, int32_t pdiv){

    if ((pdiv == 2) || ((pdiv == 4) && (pmul == 2))) {
        LERP_MATRIX(LERP_1_2);
    } else if (pdiv == 4) {

        if (pmul == 1) {
            LERP_MATRIX(LERP_1_4);
        } else {
            LERP_MATRIX(LERP_3_4);
        }

    } else {
        int32_t t = pmul * FixedInvU(pdiv) >> 8;
        LERP_MATRIX(LERP_SLOW);
    }
}

void matrixSetIdentity_c(Matrix &m) {
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

void matrixSetBasis_c(Matrix &dst, const Matrix &src)
{
    dst.e00 = src.e00;
    dst.e01 = src.e01;
    dst.e02 = src.e02;

    dst.e10 = src.e10;
    dst.e11 = src.e11;
    dst.e12 = src.e12;

    dst.e10 = src.e10;
    dst.e11 = src.e11;
    dst.e12 = src.e12;
}

#define MATRIX_TRANS(m,x,y,z)\
    int32_t tx = DP33(m.e00, m.e01, m.e02, x, y, z);\
    int32_t ty = DP33(m.e10, m.e11, m.e12, x, y, z);\
    int32_t tz = DP33(m.e20, m.e21, m.e22, x, y, z);


// Dot product of two 4D vectors (ax, ay, az, aw) and (bx, by, bz)
#define DP43(ax,ay,az,aw,bx,by,bz)  (ax * bx + ay * by + az * bz + (aw << MATRIX_FIXED_SHIFT))

// Dot product of two 3D vectors (ax, ay, az) and (bx, by, bz)
#define DP33(ax,ay,az,bx,by,bz)     (ax * bx + ay * by + az * bz)

void matrixTranslateRel_c(Matrix&m, int32_t x, int32_t y, int32_t z)
{
    MATRIX_TRANS(m, x, y, z);
    m.e03 += tx;
    m.e13 += ty;
    m.e23 += tz;
}

void matrixTranslateAbs_c(Matrix& m, int32_t x, int32_t y, int32_t z)
{
    x -= gCameraViewPos.x;
    y -= gCameraViewPos.y;
    z -= gCameraViewPos.z;
    MATRIX_TRANS(m, x, y, z);
    m.e03 = tx;
    m.e13 = ty;
    m.e23 = tz;
}

void matrixTranslateSet_c(Matrix& m, int32_t x, int32_t y, int32_t z)
{
    MATRIX_TRANS(m, x, y, z);
    m.e03 = tx;
    m.e13 = ty;
    m.e23 = tz;
}

IWRAM_CODE void matrixRotateX_c(Matrix& m, int32_t angle)
{
    // asm("msr cpsr, #0x11");
    int32_t s, c;
    sincos(angle, s, c);

    X_ROTXY(m.e02, m.e01, s, c);
    X_ROTXY(m.e12, m.e11, s, c);
    X_ROTXY(m.e22, m.e21, s, c);
    // asm("msr cpsr, #0x1F");
}

void matrixRotateY_c(Matrix& m, int32_t angle)
{
    int32_t s, c;
    sincos(angle, s, c);

    X_ROTXY(m.e00, m.e02, s, c);
    X_ROTXY(m.e10, m.e12, s, c);
    X_ROTXY(m.e20, m.e22, s, c);
}

void matrixRotateZ_c(Matrix& m, int32_t angle)
{
    int32_t s, c;
    sincos(angle, s, c);

    X_ROTXY(m.e01, m.e00, s, c);
    X_ROTXY(m.e11, m.e10, s, c);
    X_ROTXY(m.e21, m.e20, s, c);
}

void matrixRotateYQ_c(Matrix& m, int32_t quadrant)
{
    if (quadrant == 2)
        return;

    if (quadrant == 0) {
        m.e00 = -m.e00;
        m.e10 = -m.e10;
        m.e20 = -m.e20;
        m.e02 = -m.e02;
        m.e12 = -m.e12;
        m.e22 = -m.e22;
    } else if (quadrant == 1) {
        int32_t e0 = m.e02;
        int32_t e1 = m.e12;
        int32_t e2 = m.e22;

        m.e02 = -m.e00;
        m.e12 = -m.e10;
        m.e22 = -m.e20;

        m.e00 = e0;
        m.e10 = e1;
        m.e20 = e2;
    } else {
        int32_t e0 = m.e02;
        int32_t e1 = m.e12;
        int32_t e2 = m.e22;

        m.e02 = m.e00;
        m.e12 = m.e10;
        m.e22 = m.e20;

        m.e00 = -e0;
        m.e10 = -e1;
        m.e20 = -e2;
    }
}

void matrixRotateYXZ_c(Matrix& m, int32_t angleX, int32_t angleY, int32_t angleZ)
{
    if (angleY) matrixRotateY_c(m, angleY);
    if (angleX) matrixRotateX_c(m, angleX);
    if (angleZ) matrixRotateZ_c(m, angleZ);
}


