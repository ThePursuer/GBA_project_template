#ifndef FIXED_PRECISION7_H
#define FIXED_PRECISION7_H

#include <cstdint>
#include <sys/cdefs.h>

#include <gba_base.h>
#include <gba_systemcalls.h>

#include "division_table_7_16.h"

typedef int16_t fix7_t;  // signed 10.6 fixed point
__always_inline fix7_t int_to_fix7(int a){
    return a << 7;
};
__always_inline int fix7_to_int(fix7_t a){
    return a >> 7;
};
__always_inline int fix7_to_int_rounded(fix7_t a){
    return (a + 64) >> 7;
};
__always_inline int32_t fix7_mul(int32_t a, int32_t b){
    return ((a >> 4) * (b >> 3));
};
__always_inline fix7_t fix7_div(fix7_t a, fix7_t b){
    if(b < 0)
        return -((uint32_t(a) * division_table_7_16[DIV_TABLE_7_16_INDEX(b)]) >> (7 + DIVISION_TABLE_7_16_PRECISION));
    return (a * division_table_7_16[DIV_TABLE_7_16_INDEX(b)]) >> (7 + DIVISION_TABLE_7_16_PRECISION);
};
inline fix7_t fix7_sqrt(fix7_t a){
    return static_cast<int16_t>(Sqrt(static_cast<uint32_t>(a << 1))) << 3;
};
inline fix7_t fix7_arctan(fix7_t a){
    a = static_cast<fix7_t>(ArcTan(static_cast<int16_t>(a >> 7)));
    return a << 7;
};

#endif // FIXED_PRECISION7_H