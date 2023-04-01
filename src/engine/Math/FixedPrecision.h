#pragma once 

#include <cmath>

#include <gba_types.h>

namespace FixedPrecision {

using Fixed = s32;

constexpr int FIXED_SHIFT = 12;
constexpr Fixed FIXED_ONE = 1 << FIXED_SHIFT;

inline Fixed intToFixed(int value) {
    return value << FIXED_SHIFT;
}

inline int fixedToInt(Fixed value) {
    return value >> FIXED_SHIFT;
}

inline Fixed floatToFixed(float value) {
    return static_cast<Fixed>(value * FIXED_ONE);
}

inline float fixedToFloat(Fixed value) {
    return static_cast<float>(value) / FIXED_ONE;
}

inline Fixed add(const Fixed& a, const Fixed& b) {
    return a + b;
}

inline Fixed subtract(const Fixed& a, const Fixed& b) {
    return a - b;
}

inline Fixed multiply(const Fixed& a, const Fixed& b) {
    return (a * b) >> FIXED_SHIFT;
}

inline Fixed divide(const Fixed& a, const Fixed& b) {
    return (a << FIXED_SHIFT) / b;
}

inline Fixed sqrt(const Fixed& value) {
    return floatToFixed(std::sqrt(fixedToFloat(value)));
}

}  // namespace FixedPrecision