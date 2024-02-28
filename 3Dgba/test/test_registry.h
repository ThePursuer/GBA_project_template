#ifndef TEST_REGISTRY_H
#define TEST_REGISTRY_H

#include <gba_os/SimpleOutputStream.h>

bool sort_inplace_test(Gba_os::console::SimpleOutputStream& err);

bool fix7_test(Gba_os::console::SimpleOutputStream& err);
bool fix14_test(Gba_os::console::SimpleOutputStream& err);
bool Vector2_test(Gba_os::console::SimpleOutputStream& err);

bool matrixLerp_test(Gba_os::console::SimpleOutputStream& err);
bool matrixSetBasis_test(Gba_os::console::SimpleOutputStream& err);

#endif // TEST_REGISTRY_H
