#ifndef TEST_REGISTRY_H
#define TEST_REGISTRY_H

#include <sstream>

bool matrixSetBasis_asm_test(std::stringstream& err);
bool sort_inplace_test(std::stringstream& err);
bool fix7_test(std::stringstream& err);
bool assem_test(std::stringstream& err);

bool matrix_time_test(std::stringstream& err);

#endif // TEST_REGISTRY_H
