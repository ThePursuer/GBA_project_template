#ifndef DATA_TYPES_3DGBA_H
#define DATA_TYPES_3DGBA_H

#include <cstdint>

struct vector3s_t
{
    int16_t x, y, z;

    vector3s_t operator+(const vector3s_t& other) const
    {
        return { x + other.x, y + other.y, z + other.z };
    }

    vector3s_t operator-(const vector3s_t& other) const
    {
        return { x - other.x, y - other.y, z - other.z };
    }

    bool operator==(const vector3s_t& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }
};

struct vector3i_t
{
    int32_t x, y, z;

    vector3i_t operator+(const vector3i_t& other) const
    {
        return { x + other.x, y + other.y, z + other.z };
    }

    vector3i_t operator-(const vector3i_t& other) const
    {
        return { x - other.x, y - other.y, z - other.z };
    }

    bool operator==(const vector3i_t& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }
};


#endif // DATA_TYPES_3DGBA_H
