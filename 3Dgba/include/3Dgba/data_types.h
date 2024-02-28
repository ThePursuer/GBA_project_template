#ifndef DATA_TYPES_3DGBA_H
#define DATA_TYPES_3DGBA_H

#include <cstdint>
#include <array>

#include "3Dgba/math/fix7.h"
#include "3Dgba/math/fix14.h"

template<template<typename...> class TT, typename T>
struct is_instantiation_of : std::false_type { };

template<template<typename...> class TT, typename... Ts>
struct is_instantiation_of<TT, TT<Ts...>> : std::true_type { };

template <typename T>
struct Vector3 {
    static_assert(std::is_arithmetic<T>::value, "Vector2 can only be used with arithmetic types");
    T x, y, z;

    inline Vector3 operator+(const Vector3& other) const
    {
        return { x + other.x, y + other.y, z + other.z };
    }
    inline Vector3 operator-(const Vector3& other) const
    {
        return { x - other.x, y - other.y, z - other.z };
    }
    inline bool operator==(const Vector3& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }
    inline bool operator!=(const Vector3& other) const
    {
        return x != other.x || y != other.y || z != other.z;
    }
    inline Vector3& operator=(const Vector3& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
};

template <typename T>
struct Vector2{
    static_assert(std::is_arithmetic<T>::value, "Vector2 can only be used with arithmetic types");
    static_assert(sizeof(T) <= sizeof(uint32_t), "Vector2 can only be used with types that are 32 bits or less in size.");
    T x, y;

    inline Vector2 operator+(const Vector2& other) const
    {
        return { x + other.x, y + other.y };
    }
    inline Vector2 operator-(const Vector2& other) const
    {
        return { x - other.x, y - other.y };
    }
    inline bool operator==(const Vector2& other) const
    {
        if constexpr (sizeof(T) == sizeof(uint32_t))
            return *(uint64_t*)this == *(uint64_t*)&other;
        else if constexpr (sizeof(T) == sizeof(uint16_t))
            return *(uint32_t*)this == *(uint32_t*)&other;
        else if constexpr (sizeof(T) == sizeof(uint8_t))
            return *(uint16_t*)this == *(uint16_t*)&other;
    }
    inline bool operator!=(const Vector2& other) const
    {
        return x != other.x || y != other.y;
    }
    
    inline Vector2& operator=(const Vector2& other)
    {
        x = other.x;
        y = other.y;
        return *this;
    }
    inline bool operator<(const Vector2& other) const
    {
        if constexpr (sizeof(T) == sizeof(uint32_t))
            return *(uint64_t*)this < *(uint64_t*)&other;
        else if constexpr (sizeof(T) == sizeof(uint16_t))
            return *(uint32_t*)this < *(uint32_t*)&other;
        else if constexpr (sizeof(T) == sizeof(uint8_t))
            return *(uint16_t*)this < *(uint16_t*)&other;
    }
    inline bool operator>(const Vector2& other) const
    {
        if constexpr (sizeof(T) == sizeof(uint32_t))
            return *(uint64_t*)this > *(uint64_t*)&other;
        else if constexpr (sizeof(T) == sizeof(uint16_t))
            return *(uint32_t*)this > *(uint32_t*)&other;
        else if constexpr (sizeof(T) == sizeof(uint8_t))
            return *(uint16_t*)this > *(uint16_t*)&other;
    }
    inline bool operator<=(const Vector2& other) const
    {
        if constexpr (sizeof(T) == sizeof(uint32_t))
            return *(uint64_t*)this <= *(uint64_t*)&other;
        else if constexpr (sizeof(T) == sizeof(uint16_t))
            return *(uint32_t*)this <= *(uint32_t*)&other;
        else if constexpr (sizeof(T) == sizeof(uint8_t))
            return *(uint16_t*)this <= *(uint16_t*)&other;
    }
    inline bool operator>=(const Vector2& other) const
    {
        if constexpr (sizeof(T) == sizeof(uint32_t))
            return *(uint64_t*)this >= *(uint64_t*)&other;
        else if constexpr (sizeof(T) == sizeof(uint16_t))
            return *(uint32_t*)this >= *(uint32_t*)&other;
        else if constexpr (sizeof(T) == sizeof(uint8_t))
            return *(uint16_t*)this >= *(uint16_t*)&other;
    }
};

template <typename T>
struct Triangle{
    static_assert(is_instantiation_of<Vector2, T>::value, "Template must be of type Vector2 or Vector3");
    std::array<T, 3> verticies;
    short z;

    inline void sort(){
        if(verticies[0] > verticies[1])
            std::swap(verticies[0], verticies[1]);
        if(verticies[0] > verticies[2])
            std::swap(verticies[0], verticies[2]);
        if(verticies[1] > verticies[2])
            std::swap(verticies[1], verticies[2]);
    }
};


// #define ALIGN4      __attribute__((aligned(4)))
// #define ALIGN8      __attribute__((aligned(8)))
// #define ALIGN16     __attribute__((aligned(16)))

#endif // DATA_TYPES_3DGBA_H
