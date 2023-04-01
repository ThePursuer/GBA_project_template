#include "engine/Math/Math.h"

namespace Math{

float sqrtFloat(float value) {
    if (value <= 0.0f) {
        return 0.0f;
    }

    float x = value;
    float y = 1.0f;
    float epsilon = 1e-2f; // Tolerance for the difference between successive approximations

    while (x - y > epsilon) {
        x = (x + y) / 2.0f;
        y = value / x;
    }

    return x;
}

}