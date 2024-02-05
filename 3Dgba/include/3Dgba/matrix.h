#ifndef MATRIX_3DGBA_H
#define MATRIX_3DGBA_H

#include <cmath>
#include <cstdint>
#include <string>
#include <iostream>
#include <sstream>

#include "3Dgba/data_types.h"
#include "3Dgba/math.h"

#define ANGLE_360       0x10000
#define ANGLE_0         0
#define ANGLE_1         (ANGLE_360 / 360)
#define ANGLE_45        (ANGLE_360 / 8)      // != 45 * ANGLE_1 !!!
#define ANGLE_90        (ANGLE_360 / 4)      // != 90 * ANGLE_1 !!!
#define ANGLE_180      -(ANGLE_360 / 2)      // INT16_MIN
#define ANGLE(x)        ((x) * ANGLE_1)
#define ANGLE_SHIFT_45  13
#define ANGLE_SHIFT_90  14
#define ANGLE_SHIFT_180 15

struct Matrix
{
    fix14_t e00, e01, e02, e03;
    fix14_t e10, e11, e12, e13;
    fix14_t e20, e21, e22, e23;
};

Matrix& get_matrix_ptr();

inline bool operator==(const Matrix& lhs, const Matrix& rhs)
{
    return lhs.e00 == rhs.e00 && lhs.e01 == rhs.e01 && lhs.e02 == rhs.e02 && lhs.e03 == rhs.e03 &&
           lhs.e10 == rhs.e10 && lhs.e11 == rhs.e11 && lhs.e12 == rhs.e12 && lhs.e13 == rhs.e13 &&
           lhs.e20 == rhs.e20 && lhs.e21 == rhs.e21 && lhs.e22 == rhs.e22 && lhs.e23 == rhs.e23;
}

inline std::string matrix_to_string(const Matrix& matrix)
{
    std::stringstream ss;
    ss << fix14_to_int(matrix.e00) << ", " << fix14_to_int(matrix.e01) << ", " << fix14_to_int(matrix.e02) << ", " << fix14_to_int(matrix.e03) << std::endl;
    ss << fix14_to_int(matrix.e10) << ", " << fix14_to_int(matrix.e11) << ", " << fix14_to_int(matrix.e12) << ", " << fix14_to_int(matrix.e13) << std::endl;
    ss << fix14_to_int(matrix.e20) << ", " << fix14_to_int(matrix.e21) << ", " << fix14_to_int(matrix.e22) << ", " << fix14_to_int(matrix.e23) << std::endl;
    return ss.str();
}

inline std::string matrix_to_string_raw(const Matrix& matrix)
{
    std::stringstream ss;
    ss << matrix.e00 << ", " << matrix.e01 << ", " << matrix.e02 << ", " << matrix.e03 << std::endl;
    ss << matrix.e10 << ", " << matrix.e11 << ", " << matrix.e12 << ", " << matrix.e13 << std::endl;
    ss << matrix.e20 << ", " << matrix.e21 << ", " << matrix.e22 << ", " << matrix.e23 << std::endl;
    return ss.str();
}

#ifdef __cplusplus
extern "C" {
#endif
// Function declarations
void matrixLerp_c(const Matrix &n, Matrix&m, int32_t pmul, int32_t pdiv);
void matrixTranslateRel_c(Matrix &m, int32_t x, int32_t y, int32_t z);
void matrixTranslateAbs_c(Matrix& m, vector3i_t& cameraPos, int32_t x, int32_t y, int32_t z);
void matrixTranslateSet_c(Matrix &m, int32_t x, int32_t y, int32_t z);
void matrixRotateX_c(Matrix &m, int32_t angle);
void matrixRotateY_c(Matrix &m, int32_t angle);
void matrixRotateZ_c(Matrix &m, int32_t angle);
void matrixRotateYQ_c(Matrix &m, int32_t quadrant);
void matrixRotateYXZ_c(Matrix &m, int32_t angleX, int32_t angleY, int32_t angleZ);

inline void matrixSetBasis_c(Matrix &dst, const Matrix &src)
{
    dst.e00 = src.e00;
    dst.e01 = src.e01;
    dst.e02 = src.e02;

    dst.e10 = src.e10;
    dst.e11 = src.e11;
    dst.e12 = src.e12;

    dst.e10 = src.e10;
    dst.e11 = src.e11;
    dst.e12 = src.e12;
}

inline void matrixSetIdentity_c(Matrix &m) {
    m.e00 = 0x4000;
    m.e01 = 0;
    m.e02 = 0;
    m.e03 = 0;

    m.e10 = 0;
    m.e11 = 0x4000;
    m.e12 = 0;
    m.e13 = 0;

    m.e20 = 0;
    m.e21 = 0;
    m.e22 = 0x4000;
    m.e23 = 0;
}

#define ENCODE_ANGLES(x, y, z) \
    (((x >> 2) & 0x3FF0) << 16) | (((y >> 12) & 0x000F) << 16) | (((y << 4) & 0xFC00) | ((z >> 6) & 0x03FF))

/**
 * @brief Performs a frame matrix transformation.
 *
 * This function performs a frame matrix transformation based on the provided position and angles.
 * The operation is performed in assembly for performance reasons.
 *
 * @param pos Pointer to the position vector. The position vector should be a vector3 with shorts.
 * @param angles Packed angles for rotation. The angles are packed into a single 32bit integer, with the X angle in the least significant bits, the Y angle in the middle, and the Z angle in the most significant bits.
 *
 * @note This function modifies the position vector and the angles in place.
 */
void matrixFrame_asm(const vector3s_t& pos, const uint32_t& angles);

/**
 * @brief Performs a linear interpolation between two matrices.
 *
 * This function performs a linear interpolation between two matrices based on the provided multiplier and divisor.
 * The operation is performed in assembly for performance reasons.
 *
 * @param multiplier The multiplier for the interpolation.
 * @param divisor The divisor for the interpolation.
 *
 * @note This function modifies the global matrix pointer in place.
 */
void matrixLerp_asm(Matrix& m, int multiplier, int divisor); // unused

/**
 * @brief Pushes the current matrix onto the matrix stack.
 *
 * This function pushes the current matrix onto the matrix stack, effectively saving the current matrix state.
 * The operation is performed in assembly for performance reasons.
 *
 * @note This function modifies the global matrix pointer in place.
 */
void matrixPush_asm(void);

/**
 * @brief Rotates the current matrix around the X axis.
 *
 * This function rotates the current matrix around the X axis by a specified angle.
 * The operation is performed in assembly for performance reasons.
 *
 * @param angle The angle to rotate by, in degrees.
 */
int matrixRotateX_asm(int angle);

/**
 * @brief Rotates the current matrix around the Y axis.
 *
 * This function rotates the current matrix around the Y axis by a specified angle.
 * The operation is performed in assembly for performance reasons.
 *
 * @param angle The angle to rotate by, in degrees.
 */
void matrixRotateY_asm(int angle);

/**
 * @brief Rotates the current matrix around the Z axis.
 *
 * This function rotates the current matrix around the Z axis by a specified angle.
 * The operation is performed in assembly for performance reasons.
 *
 * @param angle The angle to rotate by, in degrees.
 */
void matrixRotateZ_asm(int angle);

/**
 * @brief Rotates the current matrix around the Y, X, and Z axes.
 *
 * This function rotates the current matrix around the Y, X, and Z axes by specified angles.
 * The operation is performed in assembly for performance reasons.
 *
 * @param angleX The angle to rotate around the X axis, in degrees.
 * @param angleY The angle to rotate around the Y axis, in degrees.
 * @param angleZ The angle to rotate around the Z axis, in degrees.
 */
void matrixRotateYXZ_asm(int angleX, int angleY, int angleZ);

/**
 * @brief Rotates the current matrix around the Y axis by a quarter turn.
 *
 * This function rotates the current matrix around the Y axis by a quarter turn.
 * The operation is performed in assembly for performance reasons.
 *
 * @param q The number of quarter turns to rotate by.
 */
void matrixRotateYQ_asm(int q);

/**
 * @brief Sets the basis of a matrix.
 *
 * This function sets the basis of a matrix based on the provided source matrix.
 * The operation is performed in assembly for performance reasons.
 *
 * @param dst Pointer to the destination matrix. The matrix should be a 3x4 matrix of integers.
 * @param src Pointer to the source matrix. The matrix should be a 3x4 matrix of integers.
 *
 * @note This function modifies the destination matrix in place.
 */
void matrixSetBasis_asm(Matrix& dst, const Matrix& src);

/**
 * @brief Sets the current matrix to the identity matrix.
 *
 * This function sets the current matrix to the identity matrix.
 * The operation is performed in assembly for performance reasons.
 *
 * @note This function modifies the global matrix pointer in place.
 */
void matrixSetIdentity_asm(Matrix& matrix);

/**
 * @brief Translates the current matrix relatively.
 *
 * This function translates the current matrix relatively based on the provided x, y, and z coordinates.
 * The operation is performed in assembly for performance reasons.
 *
 * @param x The x coordinate for the translation.
 * @param y The y coordinate for the translation.
 * @param z The z coordinate for the translation.
 *
 * @note This function modifies the global matrix pointer in place.
 */
void matrixTranslateRel_asm(int x, int y, int z);

/**
 * @brief Translates the current matrix absolutely.
 *
 * This function translates the current matrix absolutely based on the provided x, y, and z coordinates.
 * The operation is performed in assembly for performance reasons.
 *
 * @param x The x coordinate for the translation.
 * @param y The y coordinate for the translation.
 * @param z The z coordinate for the translation.
 *
 * @note This function modifies the global matrix pointer in place.
 */
void matrixTranslateAbs_asm(int x, int y, int z, vector3i_t* pos);

/**
 * @brief Sets the translation of the current matrix.
 *
 * This function sets the translation of the current matrix based on the provided x, y, and z coordinates.
 * The operation is performed in assembly for performance reasons.
 *
 * @param x The x coordinate for the translation.
 * @param y The y coordinate for the translation.
 * @param z The z coordinate for the translation.
 *
 * @note This function modifies the global matrix pointer in place.
 */
void matrixTranslateSet_asm(int x, int y, int z);


#ifdef __cplusplus
} // extern "C"
#endif

#endif // MATRIX_3DGBA_H
