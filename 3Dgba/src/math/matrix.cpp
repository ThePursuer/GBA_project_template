#include "3Dgba/math.h"
#include "3Dgba/matrix.h"

void matrixSetIdentity_c()
{
    Matrix &m = matrixGet();

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

void matrixLerp_c(const Matrix &n, int32 pmul, int32 pdiv)
{
    Matrix &m = matrixGet();

    if ((pdiv == 2) || ((pdiv == 4) && (pmul == 2))) {
        LERP_MATRIX(LERP_1_2);
    } else if (pdiv == 4) {

        if (pmul == 1) {
            LERP_MATRIX(LERP_1_4);
        } else {
            LERP_MATRIX(LERP_3_4);
        }

    } else {
        int32 t = pmul * FixedInvU(pdiv) >> 8;
        LERP_MATRIX(LERP_SLOW);
    }
}

#define MATRIX_TRANS(x,y,z)\
    Matrix &m = matrixGet();\
    int32 tx = DP33(m.e00, m.e01, m.e02, x, y, z);\
    int32 ty = DP33(m.e10, m.e11, m.e12, x, y, z);\
    int32 tz = DP33(m.e20, m.e21, m.e22, x, y, z);

void matrixTranslateRel_c(int32 x, int32 y, int32 z)
{
    MATRIX_TRANS(x, y, z);
    m.e03 += tx >> MATRIX_FIXED_SHIFT;
    m.e13 += ty >> MATRIX_FIXED_SHIFT;
    m.e23 += tz >> MATRIX_FIXED_SHIFT;
}

void matrixTranslateAbs_c(int32 x, int32 y, int32 z)
{
    x -= gCameraViewPos.x;
    y -= gCameraViewPos.y;
    z -= gCameraViewPos.z;
    MATRIX_TRANS(x, y, z);
    m.e03 = tx >> MATRIX_FIXED_SHIFT;
    m.e13 = ty >> MATRIX_FIXED_SHIFT;
    m.e23 = tz >> MATRIX_FIXED_SHIFT;
}

void matrixTranslateSet_c(int32 x, int32 y, int32 z)
{
    MATRIX_TRANS(x, y, z);
    m.e03 = tx >> MATRIX_FIXED_SHIFT;
    m.e13 = ty >> MATRIX_FIXED_SHIFT;
    m.e23 = tz >> MATRIX_FIXED_SHIFT;
}

void matrixRotateX_c(int32 angle)
{
    int32 s, c;
    sincos(angle, s, c);

    Matrix &m = matrixGet();
    X_ROTXY(m.e02, m.e01, s, c);
    X_ROTXY(m.e12, m.e11, s, c);
    X_ROTXY(m.e22, m.e21, s, c);
}

void matrixRotateY_c(int32 angle)
{
    int32 s, c;
    sincos(angle, s, c);

    Matrix &m = matrixGet();
    X_ROTXY(m.e00, m.e02, s, c);
    X_ROTXY(m.e10, m.e12, s, c);
    X_ROTXY(m.e20, m.e22, s, c);
}

void matrixRotateZ_c(int32 angle)
{
    int32 s, c;
    sincos(angle, s, c);

    Matrix &m = matrixGet();
    X_ROTXY(m.e01, m.e00, s, c);
    X_ROTXY(m.e11, m.e10, s, c);
    X_ROTXY(m.e21, m.e20, s, c);
}

void matrixRotateYQ_c(int32 quadrant)
{
    if (quadrant == 2)
        return;

    Matrix &m = matrixGet();

    if (quadrant == 0) {
        m.e00 = -m.e00;
        m.e10 = -m.e10;
        m.e20 = -m.e20;
        m.e02 = -m.e02;
        m.e12 = -m.e12;
        m.e22 = -m.e22;
    } else if (quadrant == 1) {
        int32 e0 = m.e02;
        int32 e1 = m.e12;
        int32 e2 = m.e22;

        m.e02 = -m.e00;
        m.e12 = -m.e10;
        m.e22 = -m.e20;

        m.e00 = e0;
        m.e10 = e1;
        m.e20 = e2;
    } else {
        int32 e0 = m.e02;
        int32 e1 = m.e12;
        int32 e2 = m.e22;

        m.e02 = m.e00;
        m.e12 = m.e10;
        m.e22 = m.e20;

        m.e00 = -e0;
        m.e10 = -e1;
        m.e20 = -e2;
    }
}

void matrixRotateYXZ_c(int32 angleX, int32 angleY, int32 angleZ)
{
    if (angleY) matrixRotateY(angleY);
    if (angleX) matrixRotateX(angleX);
    if (angleZ) matrixRotateZ(angleZ);
}

void matrixFrame_c(const void* pos, const void* angles)
{
    int16 aX, aY, aZ;
    DECODE_ANGLES(*(uint32*)angles, aX, aY, aZ);

    uint32 xy = ((uint32*)pos)[0];
    uint32 zu = ((uint32*)pos)[1];

#ifdef CPU_BIG_ENDIAN
    int32 posX = int16(xy >> 16);
    int32 posY = int16(xy & 0xFFFF);
    int32 posZ = int16(zu >> 16);
#else
    int32 posX = int16(xy & 0xFFFF);
    int32 posY = int16(xy >> 16);
    int32 posZ = int16(zu & 0xFFFF);
#endif

    matrixTranslateRel(posX, posY, posZ);
    matrixRotateYXZ(aX, aY, aZ);
}

void boxTranslate_c(AABBi &box, int32 x, int32 y, int32 z)
{
    box.minX += x;
    box.maxX += x;
    box.minY += y;
    box.maxY += y;
    box.minZ += z;
    box.maxZ += z;
}

void boxRotateYQ_c(AABBi &box, int32 quadrant)
{
    if (quadrant == 2)
        return;

    int32 minX = box.minX;
    int32 maxX = box.maxX;
    int32 minZ = box.minZ;
    int32 maxZ = box.maxZ;

    if (quadrant == 3) {
        box.minX = minZ;
        box.maxX = maxZ;
        box.minZ = -maxX;
        box.maxZ = -minX;
    } else if (quadrant == 1) {
        box.minX = -maxZ;
        box.maxX = -minZ;
        box.minZ = minX;
        box.maxZ = maxX;
    } else if (quadrant == 0) {
        box.minX = -maxX;
        box.maxX = -minX;
        box.minZ = -maxZ;
        box.maxZ = -minZ;
    }
}
#endif

void matrixFrameLerp(const void* pos, const void* anglesA, const void* anglesB, int32 delta, int32 rate)
{
    int16 aX, aY, aZ;
    int16 bX, bY, bZ;

    DECODE_ANGLES(*(uint32*)anglesA, aX, aY, aZ);
    DECODE_ANGLES(*(uint32*)anglesB, bX, bY, bZ);

    uint32 xy = ((uint32*)pos)[0];
    uint32 zu = ((uint32*)pos)[1];

#ifdef CPU_BIG_ENDIAN
    int32 posX = int16(xy >> 16);
    int32 posY = int16(xy & 0xFFFF);
    int32 posZ = int16(zu >> 16);
#else
    int32 posX = int16(xy & 0xFFFF);
    int32 posY = int16(xy >> 16);
    int32 posZ = int16(zu & 0xFFFF);
#endif

    matrixTranslateRel(posX, posY, posZ);
    
    matrixPush();
    matrixRotateYXZ(bX, bY, bZ);
    matrixPop();

    matrixRotateYXZ(aX, aY, aZ);

    matrixLerp(*(gMatrixPtr + 1), delta, rate);
}

void matrixSetView(const vec3i &pos, int32 angleX, int32 angleY)
{
    int32 sx, cx;
    int32 sy, cy;

    sincos(angleX, sx, cx);
    sincos(angleY, sy, cy);

    Matrix &m = matrixGet();

    m.e00 = cy;
    m.e01 = 0;
    m.e02 = -sy;
    m.e03 = 0;

    m.e10 = (sx * sy) >> FIXED_SHIFT;
    m.e11 = cx;
    m.e12 = (sx * cy) >> FIXED_SHIFT;
    m.e13 = 0;

    m.e20 = (cx * sy) >> FIXED_SHIFT;
    m.e21 = -sx;
    m.e22 = (cx * cy) >> FIXED_SHIFT;
    m.e23 = 0;

    gCameraViewPos = pos;
}