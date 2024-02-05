#include "3Dgba/math.h"
#include "3Dgba/matrix.h"

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