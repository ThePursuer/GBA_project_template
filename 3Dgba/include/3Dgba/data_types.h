#ifndef DATA_TYPES_3DGBA_H
#define DATA_TYPES_3DGBA_H

#include <cstdint>
#include <array>

#include "3Dgba/math.h"

struct Vector3s_t
{
    int16_t x, y, z;

    Vector3s_t operator+(const Vector3s_t& other) const
    {
        return { x + other.x, y + other.y, z + other.z };
    }

    Vector3s_t operator-(const Vector3s_t& other) const
    {
        return { x - other.x, y - other.y, z - other.z };
    }

    bool operator==(const Vector3s_t& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }
};

struct Vector3i_t
{
    fix14_t x, y, z;

    Vector3i_t operator+(const Vector3i_t& other) const
    {
        return { x + other.x, y + other.y, z + other.z };
    }

    Vector3i_t operator-(const Vector3i_t& other) const
    {
        return { x - other.x, y - other.y, z - other.z };
    }

    bool operator==(const Vector3i_t& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }
};

struct Vector2s_t
{
    int16_t x, y;

    Vector2s_t operator+(const Vector2s_t& other) const
    {
        return { x + other.x, y + other.y };
    }

    Vector2s_t operator-(const Vector2s_t& other) const
    {
        return { x - other.x, y - other.y };
    }

    bool operator==(const Vector2s_t& other) const
    {
        return x == other.x && y == other.y;
    }
};

struct Vector2c_t{
    int8_t x, y;

    Vector2c_t operator+(const Vector2c_t& other) const
    {
        return { x + other.x, y + other.y };
    }

    Vector2c_t operator-(const Vector2c_t& other) const
    {
        return { x - other.x, y - other.y };
    }

    bool operator==(const Vector2c_t& other) const
    {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Vector2c_t& other) const
    {
        return x != other.x || y != other.y;
    }
};

struct Vector2i_t
{
    fix14_t x, y;

    Vector2i_t operator+(const Vector2i_t& other) const
    {
        return { x + other.x, y + other.y };
    }

    Vector2i_t operator-(const Vector2i_t& other) const
    {
        return { x - other.x, y - other.y };
    }

    bool operator==(const Vector2i_t& other) const
    {
        return x == other.x && y == other.y;
    }
};

struct Triangle2Ds_t
{
     std::array<Vector2s_t, 3> verticies;
};

struct Triangle3Di_t
{
     std::array<Vector2i_t, 3> verticies;
};

// #define ALIGN4      __attribute__((aligned(4)))
// #define ALIGN8      __attribute__((aligned(8)))
// #define ALIGN16     __attribute__((aligned(16)))

#endif // DATA_TYPES_3DGBA_H
