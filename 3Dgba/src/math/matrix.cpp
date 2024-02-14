#include "3Dgba/math/matrix.h"


#define MAX_MATRICES 8
IWRAM_DATA Matrix gMatrixStack[MAX_MATRICES];
IWRAM_DATA Matrix* gMatrixPtr = gMatrixStack;
Matrix& get_matrix_ptr() { return *gMatrixPtr; }

IWRAM_DATA Vector3i_t gCameraViewPos;

__always_inline fix14_t interpolate_slow(fix14_t a, fix14_t b, fix14_t weight){
    return a + fix14_mul((b - a), weight);
}

__always_inline fix14_t interpolate_half(fix14_t a, fix14_t b, fix14_t weight){
    return ((b + a) >> 1) + weight;
}

__always_inline fix14_t interpolate_fourth(fix14_t a, fix14_t b, fix14_t weight){
    return a + ((b - a) >> 2);
}

__always_inline fix14_t interpolate_three_fourth(fix14_t a, fix14_t b, fix14_t weight){
    return b - ((b - a) >> 2);
}

template<fix14_t (interpolate)(fix14_t a, fix14_t b, fix14_t weight)>
__always_inline void interpolate_matrix(Matrix& a, const Matrix& b, fix14_t weight){
    a.e00 = interpolate(a.e00, b.e00, weight);
    a.e01 = interpolate(a.e01, b.e01, weight);
    a.e02 = interpolate(a.e02, b.e02, weight);
    // a.e03 = interpolate(a.e03, b.e03, weight);
    a.e10 = interpolate(a.e10, b.e10, weight);
    a.e11 = interpolate(a.e11, b.e11, weight);
    a.e12 = interpolate(a.e12, b.e12, weight);
    // a.e13 = interpolate(a.e13, b.e13, weight);
    a.e20 = interpolate(a.e20, b.e20, weight);
    a.e21 = interpolate(a.e21, b.e21, weight);
    a.e22 = interpolate(a.e22, b.e22, weight);
    // a.e23 = interpolate(a.e23, b.e23, weight);
}

__attribute__((section(".iwram"), long_call)) void matrixLinearInterpolation(Matrix& a, const Matrix& b, fix14_t weight){
    interpolate_matrix<interpolate_slow>(a, b, weight);
    // switch(weight){
    //     case 0:
    //         return;
    //     case 4096: // fourth
    //         interpolate_matrix<interpolate_fourth>(a, b, weight);
    //         return;
    //     case 8192: // half
    //         interpolate_matrix<interpolate_half>(a, b, weight);
    //         return;
    //     case 12288: // 3/4
    //         interpolate_matrix<interpolate_three_fourth>(a, b, weight);
    //         return;
    //     case 16384:
    //         a = b;
    //         return;
    //     default:
    //         interpolate_matrix<interpolate_slow>(a, b, weight);
    //         return;
    // }
}

#define MATRIX_TRANS(m,x,y,z)\
    int32_t tx = DP33(m.e00, m.e01, m.e02, x, y, z);\
    int32_t ty = DP33(m.e10, m.e11, m.e12, x, y, z);\
    int32_t tz = DP33(m.e20, m.e21, m.e22, x, y, z);

IWRAM_CODE ARM_CODE void matrixTranslateRel_c(Matrix&m, int32_t x, int32_t y, int32_t z)
{
    m.e03 += DP33(m.e00, m.e01, m.e02, x, y, z);
    m.e13 += DP33(m.e10, m.e11, m.e12, x, y, z);
    m.e23 += DP33(m.e20, m.e21, m.e22, x, y, z);
}

IWRAM_CODE ARM_CODE void matrixTranslateAbs_c(Matrix& m, Vector3i_t& cameraPos, int32_t x, int32_t y, int32_t z)
{
    x -= cameraPos.x;
    y -= cameraPos.y;
    z -= cameraPos.z;
    MATRIX_TRANS(m, x, y, z);
    m.e03 = tx;
    m.e13 = ty;
    m.e23 = tz;
}

IWRAM_CODE ARM_CODE void matrixTranslateSet_c(Matrix& m, int32_t x, int32_t y, int32_t z)
{
    MATRIX_TRANS(m, x, y, z);
    m.e03 = tx;
    m.e13 = ty;
    m.e23 = tz;
}

#define sincos(x,s,c) {\
    uint32_t sc = gSinCosTable[uint32_t(x << 16) >> 20];\
    s = int32_t(sc) >> 16;\
    c = int32_t(sc) << 16 >> 16;\
}

// Any changes to the following 3 functions should be added to the inline versions below
IWRAM_CODE ARM_CODE void matrixRotateX_c(Matrix& m, int32_t angle)
{
    int32_t s, c, tmp;
    sincos(angle, s, c);

    tmp = (m.e02 * c - m.e01 * s) >> FIXED_SHIFT;
    m.e01 = ((m.e01 * c + m.e02 * s) >> FIXED_SHIFT);
    m.e02 = tmp;

    tmp = (m.e12 * c - m.e11 * s) >> FIXED_SHIFT;
    m.e11 = ((m.e11 * c + m.e12 * s) >> FIXED_SHIFT);
    m.e12 = tmp;

    tmp = (m.e22 * c - m.e21 * s) >> FIXED_SHIFT;
    m.e21 = ((m.e21 * c + m.e22 * s) >> FIXED_SHIFT);
    m.e22 = tmp;
}

IWRAM_CODE ARM_CODE void matrixRotateY_c(Matrix& m, int32_t angle)
{
    int32_t s, c, tmp;
    sincos(angle, s, c);

    tmp = (m.e00 * c - m.e02 * s) >> FIXED_SHIFT;
    m.e02 = ((m.e02 * c + m.e00 * s) >> FIXED_SHIFT);
    m.e00 = tmp;

    tmp = (m.e10 * c - m.e12 * s) >> FIXED_SHIFT;
    m.e12 = ((m.e12 * c + m.e10 * s) >> FIXED_SHIFT);
    m.e10 = tmp;

    tmp = (m.e20 * c - m.e22 * s) >> FIXED_SHIFT;
    m.e22 = ((m.e22 * c + m.e20 * s) >> FIXED_SHIFT);
    m.e20 = tmp;
}

IWRAM_CODE ARM_CODE void matrixRotateZ_c(Matrix& m, int32_t angle)
{
    int32_t s, c, tmp;
    sincos(angle, s, c);
    
    tmp = (m.e01 * c - m.e00 * s) >> FIXED_SHIFT;
    m.e00 = ((m.e00 * c + m.e01 * s) >> FIXED_SHIFT);
    m.e01 = tmp;

    tmp = (m.e11 * c - m.e10 * s) >> FIXED_SHIFT;
    m.e10 = ((m.e10 * c + m.e11 * s) >> FIXED_SHIFT);
    m.e11 = tmp;

    tmp = (m.e21 * c - m.e20 * s) >> FIXED_SHIFT;
    m.e20 = ((m.e20 * c + m.e21 * s) >> FIXED_SHIFT);
    m.e21 = tmp;
}

// These functions should be kept in sync with the non inline functions above. They are here for perfomance reasons.
__always_inline IWRAM_CODE ARM_CODE void matrixRotateX_c_inline(Matrix& m, int32_t angle)
{
    int32_t s, c, tmp;
    sincos(angle, s, c);

    tmp = (m.e02 * c - m.e01 * s) >> FIXED_SHIFT;
    m.e01 = ((m.e01 * c + m.e02 * s) >> FIXED_SHIFT);
    m.e02 = tmp;

    tmp = (m.e12 * c - m.e11 * s) >> FIXED_SHIFT;
    m.e11 = ((m.e11 * c + m.e12 * s) >> FIXED_SHIFT);
    m.e12 = tmp;

    tmp = (m.e22 * c - m.e21 * s) >> FIXED_SHIFT;
    m.e21 = ((m.e21 * c + m.e22 * s) >> FIXED_SHIFT);
    m.e22 = tmp;
}

__always_inline IWRAM_CODE ARM_CODE void matrixRotateY_c_inline(Matrix& m, int32_t angle)
{
    int32_t s, c, tmp;
    sincos(angle, s, c);

    tmp = (m.e00 * c - m.e02 * s) >> FIXED_SHIFT;
    m.e02 = ((m.e02 * c + m.e00 * s) >> FIXED_SHIFT);
    m.e00 = tmp;

    tmp = (m.e10 * c - m.e12 * s) >> FIXED_SHIFT;
    m.e12 = ((m.e12 * c + m.e10 * s) >> FIXED_SHIFT);
    m.e10 = tmp;

    tmp = (m.e20 * c - m.e22 * s) >> FIXED_SHIFT;
    m.e22 = ((m.e22 * c + m.e20 * s) >> FIXED_SHIFT);
    m.e20 = tmp;
}

__always_inline IWRAM_CODE ARM_CODE void matrixRotateZ_c_inline(Matrix& m, int32_t angle)
{
    int32_t s, c, tmp;
    sincos(angle, s, c);
    
    tmp = (m.e01 * c - m.e00 * s) >> FIXED_SHIFT;
    m.e00 = ((m.e00 * c + m.e01 * s) >> FIXED_SHIFT);
    m.e01 = tmp;

    tmp = (m.e11 * c - m.e10 * s) >> FIXED_SHIFT;
    m.e10 = ((m.e10 * c + m.e11 * s) >> FIXED_SHIFT);
    m.e11 = tmp;

    tmp = (m.e21 * c - m.e20 * s) >> FIXED_SHIFT;
    m.e20 = ((m.e20 * c + m.e21 * s) >> FIXED_SHIFT);
    m.e21 = tmp;
}

IWRAM_CODE ARM_CODE void matrixRotateYXZ_c(Matrix& m, int32_t angleX, int32_t angleY, int32_t angleZ)
{
    matrixRotateY_c_inline(m, angleY);
    matrixRotateX_c_inline(m, angleX);
    matrixRotateZ_c_inline(m, angleZ);
}

IWRAM_CODE ARM_CODE void matrixRotateYQ_c(Matrix& m, int32_t quadrant)
{
    switch(quadrant) {
        case 0:
            m.e00 = -m.e00;
            m.e10 = -m.e10;
            m.e20 = -m.e20;
            m.e02 = -m.e02;
            m.e12 = -m.e12;
            m.e22 = -m.e22;
            break;
        case 1:
            {
                int32_t e0 = m.e02;
                int32_t e1 = m.e12;
                int32_t e2 = m.e22;

                m.e02 = -m.e00;
                m.e12 = -m.e10;
                m.e22 = -m.e20;

                m.e00 = e0;
                m.e10 = e1;
                m.e20 = e2;
            }
            break;
        case 2:
            return;
        default:
            {
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
            break;
    }
}
