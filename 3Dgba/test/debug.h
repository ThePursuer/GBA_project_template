#ifndef DEBUG_H
#define DEBUG_H

#include <sstream>
#include <string>

#include <3Dgba/math/math.h>
#include <3Dgba/math/matrix.h>

inline std::string matrix_to_string(const Matrix& matrix)
{
    std::stringstream ss;
    ss << std::to_string(fix14_to_int(matrix.e00)) << ", " << std::to_string(fix14_to_int(matrix.e01)) << ", " << std::to_string(fix14_to_int(matrix.e02)) << ", " << std::to_string(fix14_to_int(matrix.e03)) << std::endl;
    ss << std::to_string(fix14_to_int(matrix.e10)) << ", " << std::to_string(fix14_to_int(matrix.e11)) << ", " << std::to_string(fix14_to_int(matrix.e12)) << ", " << std::to_string(fix14_to_int(matrix.e13)) << std::endl;
    ss << std::to_string(fix14_to_int(matrix.e20)) << ", " << std::to_string(fix14_to_int(matrix.e21)) << ", " << std::to_string(fix14_to_int(matrix.e22)) << ", " << std::to_string(fix14_to_int(matrix.e23));
    return ss.str();
}

inline std::string matrix_to_string_raw(const Matrix& matrix)
{
    std::stringstream ss;
    ss << std::to_string(matrix.e00) << ", " << std::to_string(matrix.e01) << ", " << std::to_string(matrix.e02) << ", " << std::to_string(matrix.e03) << std::endl;
    ss << std::to_string(matrix.e10) << ", " << std::to_string(matrix.e11) << ", " << std::to_string(matrix.e12) << ", " << std::to_string(matrix.e13) << std::endl;
    ss << std::to_string(matrix.e20) << ", " << std::to_string(matrix.e21) << ", " << std::to_string(matrix.e22) << ", " << std::to_string(matrix.e23);
    return ss.str();
}

#endif // DEBUG_H