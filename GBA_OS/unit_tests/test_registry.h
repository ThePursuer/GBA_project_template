#ifndef TEST_REGISTRY_H
#define TEST_REGISTRY_H

#include "gba_os/SimpleOutputStream.h"

bool gba_units_compare_test(Gba_os::console::SimpleOutputStream& err);
bool clock_test(Gba_os::console::SimpleOutputStream& err);

#endif // TEST_REGISTRY_H