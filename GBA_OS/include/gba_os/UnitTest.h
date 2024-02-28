// UnitTests.h
#ifndef UNIT_TESTS_H
#define UNIT_TESTS_H

#include <iostream>
#include <functional>
#include <array>
#include <string>
#include <sstream>

#include <gba_input.h>

#include <gba_os/CoreDefs.h>
#include <gba_os/SimpleOutputStream.h>

namespace Gba_os::test {

// Function to register a test case
void RegisterTest(const std::string& name, std::function<bool(Gba_os::console::SimpleOutputStream&)> testFunc);

// Function to run all registered test cases and capture results in the buffer
std::vector<std::string> RunTests();

// Function to display up to 20 lines from the buffer starting from a given line
void DisplayTestResults(int startLine, std::vector<std::string>& lines);

void test_results_task(Gba_os::Task& t);

} // namespace gba_os::test

#endif // UNIT_TESTS_H
