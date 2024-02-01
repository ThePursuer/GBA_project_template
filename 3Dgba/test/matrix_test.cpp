#include <3Dgba/matrix.h>

#include <gba_os/Core.h>
#include <gba_os/Clock/GbaClock.h>
#include <gba_os/QuickTimer.h>

#include <cstring>
#include <sstream>
#include <iostream>

#include <gba.h>

#include <3Dgba/math.h>

bool matrixSetBasis_asm_test(){
    Matrix m;
    m.e00 = 1;
    m.e01 = 2;
    m.e02 = 3;
    m.e10 = 4;
    m.e11 = 5;
    m.e12 = 6;
    m.e20 = 7;
    m.e21 = 8;
    m.e22 = 9;

    m.e03 = 0;
    m.e13 = 0;
    m.e23 = 0;

    Matrix n{};

    matrixSetBasis_asm(n, m);
    if (m == n)
        return true;
    return false;
}

uint32_t pack_angles(int x, int y, int z) {
    return ((x & 0x3FF) << 20) | ((y & 0x3FF) << 10) | (z & 0x3FF);
}

bool matrixRotateX_asm_test(){
    // Matrix& m = get_matrix_ptr();
    Matrix m, n;
    m.e00 = int_to_fix16(1);
    m.e01 = int_to_fix16(3);
    m.e02 = int_to_fix16(1);

    m.e10 = int_to_fix16(3);
    m.e11 = int_to_fix16(1);
    m.e12 = int_to_fix16(1);

    m.e20 = int_to_fix16(31);
    m.e21 = int_to_fix16(1);
    m.e22 = int_to_fix16(1);
    
    m.e03 = int_to_fix16(1);
    m.e13 = int_to_fix16(1);
    m.e23 = int_to_fix16(1);

    std::memcpy(&n, &m, sizeof(Matrix));
    std::stringstream ss;

    uint16_t delta;

    resetTimer();
    startTimer();
    matrixRotateX_asm(ANGLE_90, m);
    delta = stopTimer();
    ss << "Time asm: " << delta << std::endl;
    ss << matrix_to_string(m) << std::endl;

    resetTimer();
    startTimer();
    matrixRotateX_c(n, ANGLE_90);
    delta = stopTimer();
    ss << "Time c: " << delta << std::endl;
    ss << matrix_to_string(n) << std::endl;

    gba_os::raise_software_error(ss.str());
    return false;
}