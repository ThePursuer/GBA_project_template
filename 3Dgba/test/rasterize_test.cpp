#include "debug.h"

#include "3Dgba/rasterize.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <array>

#include <gba_os/QuickTimer.h>
#include <gba_os/SimpleOutputStream.h>

using namespace gba_3D;
using namespace Gba_os::console;

bool sort_inplace_test(SimpleOutputStream& err){
    Triangle<Vector2<fix14_t>> triangle = {
        { 50, 90, 10, 80, 80, 20 }
    };

    Triangle<Vector2<fix14_t>> expected = {
        { 80, 20, 10, 80, 50, 90 }
    };

    triangle.sort();

    if (triangle.verticies[0] != expected.verticies[0] || triangle.verticies[1] != expected.verticies[1] || triangle.verticies[2] != expected.verticies[2]){
        err << "Expected: " << std::to_string(expected.verticies[0].x) << ", " << std::to_string(expected.verticies[0].y) << " " << std::to_string(expected.verticies[1].x) << ", " << std::to_string(expected.verticies[1].y) << " " << std::to_string(expected.verticies[2].x) << ", " << std::to_string(expected.verticies[2].y) << std::endl;
        err << "Got: " << std::to_string(triangle.verticies[0].x) << ", " << std::to_string(triangle.verticies[0].y) << " " << std::to_string(triangle.verticies[1].x) << ", " << std::to_string(triangle.verticies[1].y) << " " << std::to_string(triangle.verticies[2].x) << ", " << std::to_string(triangle.verticies[2].y);
        return false;
    }
    return true;
}
