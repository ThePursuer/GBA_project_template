#include "Vector.h"

class Vector2D {
public:
    fix16_t x, y;

    // Constructor
    Vector2D(fix16_t x = 0, fix16_t y = 0) : x(x), y(y) {}

    // Addition of two vectors
    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(fix16_add(x, other.x), fix16_add(y, other.y));
    }

    // Subtraction of two vectors
    Vector2D operator-(const Vector2D& other) const {
        return Vector2D(fix16_sub(x, other.x), fix16_sub(y, other.y));
    }

    // Scalar multiplication
    Vector2D operator*(fix16_t scalar) const {
        return Vector2D(fix16_mul(x, scalar), fix16_mul(y, scalar));
    }

    // Dot product
    fix16_t dot(const Vector2D& other) const {
        return fix16_add(fix16_mul(x, other.x), fix16_mul(y, other.y));
    }

    // Magnitude (Length) of the vector
    fix16_t magnitude() const {
        return fix16_sqrt(fix16_add(fix16_mul(x, x), fix16_mul(y, y)));
    }

    // Normalize the vector
    Vector2D normalize() const {
        fix16_t mag = magnitude();
        // Avoid division by zero
        if (mag == 0) return Vector2D(0, 0);
        return Vector2D(fix16_div(x, mag), fix16_div(y, mag));
    }

    // Other useful operations can be added as necessary
};

class Vector3D {
public:
    fix16_t x, y, z;

    // Constructor
    Vector3D(fix16_t x = 0, fix16_t y = 0, fix16_t z = 0) : x(x), y(y), z(z) {}

    // Addition of two vectors
    Vector3D operator+(const Vector3D& other) const {
        return Vector3D(fix16_add(x, other.x), fix16_add(y, other.y), fix16_add(z, other.z));
    }

    // Subtraction of two vectors
    Vector3D operator-(const Vector3D& other) const {
        return Vector3D(fix16_sub(x, other.x), fix16_sub(y, other.y), fix16_sub(z, other.z));
    }

    // Scalar multiplication
    Vector3D operator*(fix16_t scalar) const {
        return Vector3D(fix16_mul(x, scalar), fix16_mul(y, scalar), fix16_mul(z, scalar));
    }

    // Dot product
    fix16_t dot(const Vector3D& other) const {
        return fix16_add(fix16_mul(x, other.x), fix16_add(fix16_mul(y, other.y), fix16_mul(z, other.z)));
    }

    // Cross product
    Vector3D cross(const Vector3D& other) const {
        return Vector3D(fix16_sub(fix16_mul(y, other.z), fix16_mul(z, other.y)),
                        fix16_sub(fix16_mul(z, other.x), fix16_mul(x, other.z)),
                        fix16_sub(fix16_mul(x, other.y), fix16_mul(y, other.x)));
    }

    // Magnitude (Length) of the vector
    fix16_t magnitude() const {
        return fix16_sqrt(fix16_add(fix16_mul(x, x), fix16_add(fix16_mul(y, y), fix16_mul(z, z))));
    }

    // Normalize the vector
    Vector3D normalize() const {
        fix16_t mag = magnitude();
        // Avoid division by zero
        if (mag == 0) return Vector3D(0, 0, 0);
        return Vector3D(fix16_div(x, mag), fix16_div(y, mag), fix16_div(z, mag));
    }

    // Other useful operations can be added as necessary
};
