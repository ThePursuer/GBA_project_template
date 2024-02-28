#ifndef BRANCHLESS_H
#define BRANCHLESS_H

#include <cstdlib>

#include "fix7.h"

__always_inline fix7_t branchless_abs (const fix7_t x) {
    return (x ^ (x >> 15)) - (x >> 15);
}
 
__always_inline fix7_t branchless_min(const fix7_t a, const fix7_t b){
    return b ^ ((a ^ b) & -(a < b));
}

__always_inline fix7_t branchless_max(const fix7_t a, const fix7_t b){
    return a ^ ((a ^ b) & -(a < b));
}

__always_inline fix7_t branchless_clamp(const fix7_t x, const fix7_t a, const fix7_t b){
    return branchless_max(a, branchless_min(x, b));
}

#endif // BRANCHLESS_H