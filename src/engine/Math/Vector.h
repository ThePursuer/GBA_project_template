#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>

#include "engine/Math/Math.h"

#include <stdio.h>

// Representation of a 2D vector for use in SAT
class Vector2 {
public:
    Vector2() = default;
    Vector2(float x, float y) : x(x), y(y) {}

    float x, y;
    
    Vector2 perpendicular() const {
        return Vector2(-y, x);
    }

    float lengthSquared() const {
        return x * x + y * y;
    }

    float length() const {
        return Math::sqrtFloat(lengthSquared());
    }

    Vector2 normalize() const {
        float len = length();
        if (len == 0) {
            return Vector2(0, 0);
        }
        return Vector2(x / len, y / len);
    }

    float dot(const Vector2& other) const {
        return x * other.x + y * other.y;
    }

    Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }

    Vector2 operator*(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    Vector2 operator/(float scalar) const {
        return Vector2(x / scalar, y / scalar);
    }
};

#endif // VECTOR_H
