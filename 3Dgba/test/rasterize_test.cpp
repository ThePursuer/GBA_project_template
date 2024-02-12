#include "debug.h"

#include "3Dgba/rasterize.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <array>

#include <gba_os/QuickTimer.h>

using namespace gba_3D;

bool sort_inplace_test(std::stringstream& err){
    Vector2c_t triangle[3] = {
        { 50, 90 },
        { 10, 80 },
        { 80, 20 }
    };

    Vector2c_t expected[3] = {
        { 80, 20 },
        { 10, 80 },
        { 50, 90 }
    };

    gba_3D::sort_inplace(*reinterpret_cast<vertex_int32_u(*)[3]>(&triangle));

    if (triangle[0] != expected[0] || triangle[1] != expected[1] || triangle[2] != expected[2]){
        err << "Expected: " << std::to_string(expected[0].x) << ", " << std::to_string(expected[0].y) << " " << std::to_string(expected[1].x) << ", " << std::to_string(expected[1].y) << " " << std::to_string(expected[2].x) << ", " << std::to_string(expected[2].y) << std::endl;
        err << "Got: " << std::to_string(triangle[0].x) << ", " << std::to_string(triangle[0].y) << " " << std::to_string(triangle[1].x) << ", " << std::to_string(triangle[1].y) << " " << std::to_string(triangle[2].x) << ", " << std::to_string(triangle[2].y);
        return false;
    }
    return true;
}
