#ifndef MATH_3DGBA_H
#define MATH_3DGBA_H

#include <cstdint>

#include <gba_base.h>
#include <gba_systemcalls.h>

#include "division_table.h"

extern const uint32_t gSinCosTable[4096];

typedef int32_t fix14_t;  // signed 18.14 fixed point
extern "C" fix14_t int_to_fix14(int32_t a);
extern "C" int32_t fix14_to_int(fix14_t a);
extern "C" fix14_t fix14_mul(fix14_t a, fix14_t b);
inline fix14_t fix14_mul_fast(fix14_t a, fix14_t b){
    return (a >> 7) * (b >> 7);
};
inline fix14_t fix14_div_fast(fix14_t a, fix14_t b){
    // todo: lol slow
    return (a / (b >> 10)) << 4;
};
inline fix14_t fix14_sqrt(fix14_t a){
    return static_cast<int32_t>(Sqrt(static_cast<uint32_t>(a))) << 7;
};
inline fix14_t fix14_arctan(fix14_t a){
    a = static_cast<fix14_t>(ArcTan(static_cast<int16_t>(a >> 14)));
    return a << 14;
};

typedef int16_t fix7_t;  // signed 10.6 fixed point
inline fix7_t int_to_fix7(int a){
    return a << 7;
};
inline int fix7_to_int(fix7_t a){
    return a >> 7;
};
inline int fix7_to_int_rounded(fix7_t a){
    return (a + 64) >> 7;
};
inline int32_t fix7_mul(int32_t a, int32_t b){
    return ((a >> 4) * (b >> 3));
};
inline fix7_t fix7_div(fix7_t a, fix7_t b){
    // division is complicated....
    if(b < 0)
        return -((a * division_table[DIV_TABLE_INDEX(b)]) >> (7 + DIVISION_TABLE_PRECISION));
    return (a * division_table[DIV_TABLE_INDEX(b)]) >> (7 + DIVISION_TABLE_PRECISION);
};
inline fix7_t fix7_sqrt(fix7_t a){
    return static_cast<int16_t>(Sqrt(static_cast<uint32_t>(a << 1))) << 3;
};
inline fix7_t fix7_arctan(fix7_t a){
    a = static_cast<fix7_t>(ArcTan(static_cast<int16_t>(a >> 7)));
    return a << 7;
};

#endif // MATH_3DGBA_H
