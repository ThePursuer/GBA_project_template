#include "3Dgba/matrix.h"


#define MAX_MATRICES 8
IWRAM_DATA Matrix gMatrixStack[MAX_MATRICES];
IWRAM_DATA Matrix* gMatrixPtr = gMatrixStack;
Matrix& get_matrix_ptr() { return *gMatrixPtr; }

IWRAM_DATA Vector3i_t gCameraViewPos;

#define one_third 5460
#define one_fifth 3267

#define LERP_1_2(a, b)   a = (b + a) >> 1
#define LERP_1_3(a, b)   a = a + fix14_mul_fast((b - a), one_third)
#define LERP_2_3(a, b)   a = b - fix14_mul_fast((b - a), one_third)
#define LERP_1_4(a, b)   a = a + ((b - a) >> 2)
#define LERP_3_4(a, b)   a = b - ((b - a) >> 2)
#define LERP_1_5(a, b)   a = a + fix14_mul_fast((b - a), one_fifth)
#define LERP_2_5(a, b)   a = a + fix14_mul_fast(((b - a) << 1), one_fifth)
#define LERP_3_5(a, b)   a = b - fix14_mul_fast(((b - a) << 1), one_fifth)
#define LERP_4_5(a, b)   a = b - fix14_mul_fast((b - a), one_fifth)
#define LERP_SLOW(a, b)  a = a + fix14_mul_fast((b - a), (t))

// possibly incorrect. Assembly function does not change ex3 but this does
#define LERP_ROW(lerp_func, a, b, row) \
    lerp_func(a.e##row##0, b.e##row##0); \
    lerp_func(a.e##row##1, b.e##row##1); \
    lerp_func(a.e##row##2, b.e##row##2);
    // lerp_func(a.e##row##3, b.e##row##3);

#define LERP_MATRIX(lerp_func) \
    LERP_ROW(lerp_func, m, n, 0); \
    LERP_ROW(lerp_func, m, n, 1); \
    LERP_ROW(lerp_func, m, n, 2);

IWRAM_CODE ARM_CODE void matrixLerp_c(const Matrix &n, Matrix &m, int32_t pmul, int32_t pdiv){
    switch (pdiv)
    {
    case 2: 
        switch (pmul)
        {
            case 1:
                LERP_MATRIX(LERP_1_2);
                break;
            default:
                goto slow_case;
        }
        break;
    case 3:
        switch (pmul)
        {
        case 1:
            LERP_MATRIX(LERP_1_3);
            break;
        case 2:
            LERP_MATRIX(LERP_2_3);
            break;
        default:
            goto slow_case;
        }
        break;
    case 4:
        switch (pmul)
        {
        case 1:
            LERP_MATRIX(LERP_1_4);
            break;
        case 2:
            LERP_MATRIX(LERP_1_2);
            break;
        case 3:
            LERP_MATRIX(LERP_3_4);
            break;
        default:
            goto slow_case;
        }
        break;
    case 5:
        switch (pmul) 
        {
        case 1:
            LERP_MATRIX(LERP_1_5);
            break;
        case 2:
            LERP_MATRIX(LERP_2_5);
            break;
        case 3:
            LERP_MATRIX(LERP_3_5);
            break;
        case 4:
            LERP_MATRIX(LERP_4_5);
            break;        
        default:
            goto slow_case;
        }
    default:
        goto slow_case;
        break;
    }
    return;
slow_case:
    int32_t t = fix14_div_fast(int_to_fix14(pmul), int_to_fix14(pdiv));
    LERP_MATRIX(LERP_SLOW);
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
