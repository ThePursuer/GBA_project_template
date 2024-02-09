#ifndef MATH_3DGBA_H
#define MATH_3DGBA_H

#include <cstdint>
#include <fastmath.h>

#include <gba_base.h>
#include <gba_systemcalls.h>

extern const uint32_t gSinCosTable[4096];

typedef int32_t fix14_t;  // signed 18.14 fixed point
extern "C" fix14_t int_to_fix14(int32_t a);
extern "C" int32_t fix14_to_int(fix14_t a);
extern "C" fix14_t fix14_mul(fix14_t a, fix14_t b);
inline fix14_t fix14_mul_fast(fix14_t a, fix14_t b){
    return (a >> 7) * (b >> 7);
};
inline fix14_t fix14_div_fast(fix14_t a, fix14_t b){
    return (a / (b >> 10)) << 4;
};
inline fix14_t fix14_sqrt(fix14_t a){
    return static_cast<int32_t>(Sqrt(static_cast<uint32_t>(a))) << 7;
};
inline fix14_t fix14_arctan(fix14_t a){
    a = static_cast<fix14_t>(ArcTan(static_cast<int16_t>(a >> 14)));
    return a << 14;
};

#endif // MATH_3DGBA_H
