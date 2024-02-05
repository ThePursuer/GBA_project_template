#include <3Dgba/matrix.h>

#include <gba_os/Core.h>
#include <gba_os/Clock/GbaClock.h>
#include <gba_os/QuickTimer.h>

#include <cstring>
#include <sstream>
#include <iostream>
#include <bitset>

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
    Matrix& m = get_matrix_ptr();
    Matrix n;
    n.e00 = int_to_fix14(1);
    n.e01 = int_to_fix14(2);
    n.e02 = int_to_fix14(3);
    n.e03 = int_to_fix14(4);

    n.e10 = int_to_fix14(5);
    n.e11 = int_to_fix14(6);
    n.e12 = int_to_fix14(7);
    n.e13 = int_to_fix14(8);

    n.e20 = int_to_fix14(9);
    n.e21 = int_to_fix14(10);
    n.e22 = int_to_fix14(11);
    n.e23 = int_to_fix14(12);
    m = n;

    std::stringstream ss;
    uint16_t delta;

    resetTimer();
    startTimer();
    matrixTranslateRel_c(n, 1, 1, 1);
    delta = stopTimer();
    ss << matrix_to_string(n) << std::endl;
    ss << "C++ time: " << delta << std::endl;

    resetTimer();
    startTimer();
    matrixTranslateRel_asm(1, 1, 1);
    delta = stopTimer();
    ss << matrix_to_string(m) << std::endl;
    ss << "asm switch time: " << delta << std::endl;

    gba_os::raise_software_error(ss.str());
    return false;
}