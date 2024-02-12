#include "3Dgba/division_table.h"

#include <cstdint>
#include <gba.h>
#include <cmath>

typedef int16_t fix7_t;  // signed 10.6 fixed point


inline int assem_func(int a, int b) {
    asm volatile(
        "mul %[result], %[input_a], %[input_b]\n"
        : [result] "+r" (a)
        : [input_a] "r" (a), [input_b] "r" (b)
        : "cc" // Condition code flags are affected by the add operation
    );
}

inline fix7_t int_to_fix7(int a){
    return a << 6;
};
inline int fix7_to_int(fix7_t a){
    return a >> 6;
};
inline int fix7_to_int_rounded(fix7_t a){
    return (a + 32) >> 6;
};
__always_inline void fix7_mul(fix7_t &a, fix7_t b){
    a = ((a >> 3) * (b >> 3));
};
__always_inline fix7_t fix7_mul_asm(fix7_t a, const fix7_t b){
        asm volatile(
        "asr %[input_a], %[input_a], #3\n"
        "asr %[input_b], %[input_b], #3\n"
        "mul %[result], %[input_a], %[input_b]\n"
        : [result] "+r" (a)
        : [input_a] "r" (a), [input_b] "r" (b)
        : "cc" // Condition code flags are affected by the add operation
    );
    return a;
};
inline fix7_t fix7_div(fix7_t a, fix7_t b){
    return (a * division_table[b >> 6]) >> 6;
};
