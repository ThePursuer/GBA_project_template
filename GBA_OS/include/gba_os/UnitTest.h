// UnitTests.h
#ifndef UNIT_TESTS_H
#define UNIT_TESTS_H

#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <sstream>

#include <gba_input.h>

#include <gba_os/CoreDefs.h>

namespace gba_os::test {
// Struct to represent a single test case
struct TestCase {
    std::string name;
    std::function<bool()> testFunc;
};

// Function to register a test case
void RegisterTest(const std::string& name, std::function<bool()> testFunc);

// Function to run all registered test cases and capture results in the buffer
void RunTests();

// Function to display up to 20 lines from the buffer starting from a given line
void DisplayTestResults(int startLine);

void test_results_task(gba_os::Task& t);

} // namespace gba_os::test

#endif // UNIT_TESTS_H