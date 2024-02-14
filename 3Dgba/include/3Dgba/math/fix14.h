#ifndef FIXED_PRECISION14_H
#define FIXED_PRECISION14_H

#include <cstdint>

#include <gba_base.h>
#include <gba_systemcalls.h>

#include "division_table_14_32.h"

typedef int32_t fix14_t;  // signed 18.14 fixed point
inline fix14_t int_to_fix14(int a){
    return a << 14;
};
inline int fix14_to_int(fix14_t a){
    return a >> 14;
};
inline int fix14_to_int_rounded(fix14_t a){
    return (a + 8192) >> 14;
};
inline int32_t fix14_mul(int32_t a, int32_t b){
    return ((a >> 7) * (b >> 7));
};
inline fix14_t fix14_div(fix14_t a, fix14_t b){
    if(b < 0)
        return -(((a >> 7) * division_table_14_32[DIV_TABLE_14_32_INDEX(b)]) >> (7 + DIVISION_TABLE_14_32_PRECISION));
    return ((a >> 7) * division_table_14_32[DIV_TABLE_14_32_INDEX(b)]) >> (7 + DIVISION_TABLE_14_32_PRECISION);
};
inline fix14_t fix14_sqrt(fix14_t a){
    return static_cast<int16_t>(Sqrt(static_cast<uint32_t>(a))) << 7;
};
inline fix14_t fix14_arctan(fix14_t a){
    a = static_cast<fix14_t>(ArcTan(static_cast<int16_t>(fix14_to_int_rounded(a))));
    return int_to_fix14(a);
};

#endif // FIXED_PRECISION14_H