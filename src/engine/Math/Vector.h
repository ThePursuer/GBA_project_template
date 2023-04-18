#pragma once

#include <libfixmath/fixmath.h>

#include <stdio.h>

// Representation of a 2D vector for use in SAT
class Vector2 {
public:
    Vector2() = default;
    Vector2(fix16_t x, fix16_t y) : x(x), y(y) {}
    static Vector2 fromS32(s32 x, s32 y) {return Vector2(fix16_from_int(x), fix16_from_int(y));}

    fix16_t x, y;

    Vector2 perpendicular() const {
        return Vector2(-y, x);
    }

    fix16_t lengthSquared() const {
        return fix16_add(fix16_mul(x, x), fix16_mul(y, y));
    }

    fix16_t length() const {
        return fix16_sqrt(fix16_from_int(fix16_to_int(lengthSquared())));
    }

    Vector2 normalize() const {
        fix16_t len = length();
        if (len == 0) {
            return Vector2(0, 0);
        }
        return Vector2(fix16_div(x, len), fix16_div(y, len));
    }

    fix16_t dot(const Vector2& other) const {
        return fix16_add(fix16_mul(x, other.x), fix16_mul(y, other.y));
    }

    Vector2 operator+(const Vector2& other) const {
        return Vector2(fix16_add(x, other.x), fix16_add(y, other.y));
    }

    Vector2 operator-(const Vector2& other) const {
        return Vector2(fix16_sub(x, other.x), fix16_sub(y, other.y));
    }

    Vector2 operator*(fix16_t scalar) const {
        return Vector2(fix16_mul(x, scalar), fix16_mul(y, scalar));
    }

    Vector2 operator/(fix16_t scalar) const {
        return Vector2(fix16_div(x, scalar), fix16_div(y, scalar));
    }
};
