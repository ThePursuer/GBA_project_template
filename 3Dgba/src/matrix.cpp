#include "3Dgba/matrix.h"

#include <gba.h>

#define MAX_MATRICES        8
IWRAM_DATA Matrix gMatrixStack[MAX_MATRICES];
IWRAM_DATA Matrix* gMatrixPtr = gMatrixStack;
IWRAM_DATA vector3i_t gCameraViewPos;

void matrixPop() {
    if(gMatrixPtr > gMatrixStack)
        gMatrixPtr--;
}

Matrix& get_matrix_ptr() {
    return *gMatrixPtr;
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