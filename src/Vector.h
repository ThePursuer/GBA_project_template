#ifndef VECTORS_H
#define VECTORS_H

#include <libfixmath/fixmath.h>

class Vector2D {
public:
    fix16_t x, y;

    Vector2D(fix16_t x = 0, fix16_t y = 0) : x(x), y(y) {}

    Vector2D operator+(const Vector2D& other) const;
    Vector2D operator-(const Vector2D& other) const;
    Vector2D operator*(fix16_t scalar) const;
    fix16_t dot(const Vector2D& other) const;
    fix16_t magnitude() const;
    Vector2D normalize() const;
};

class Vector3D {
public:
    fix16_t x, y, z;

    Vector3D(fix16_t x = 0, fix16_t y = 0, fix16_t z = 0) : x(x), y(y), z(z) {}

    Vector3D operator+(const Vector3D& other) const;
    Vector3D operator-(const Vector3D& other) const;
    Vector3D operator*(fix16_t scalar) const;
    fix16_t dot(const Vector3D& other) const;
    Vector3D cross(const Vector3D& other) const;
    fix16_t magnitude() const;
    Vector3D normalize() const;
};

#endif // VECTORS_H
