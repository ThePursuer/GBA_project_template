#include "gba_os/UnitTest.h"

#include "gba_os/Console.h"

#include <cstring>


namespace Gba_os::test {

using namespace Gba_os::console;

// Struct to represent a single test case
struct TestCase {
    std::string name;
    std::function<bool(Gba_os::console::SimpleOutputStream&)> testFunc;
};

// Buffer to capture test results
EWRAM_DATA char buffer[51200];
SimpleOutputStream testBuffer(buffer, sizeof(buffer));

// Global array to store all registered test cases
EWRAM_DATA std::array<TestCase, 100> testCases;
int testCount = 0;


// Function to register a test case
void RegisterTest(const std::string& name, std::function<bool(SimpleOutputStream&)> testFunc) {
    testCases[testCount].name = name;
    testCases[testCount].testFunc = testFunc;
    testCount++;
}

// Function to run all registered test cases and capture results in the buffer
std::vector<std::string> RunTests() {
    int passedCount = 0;
    int totalCount = testCount;

    char buffer[512];
    SimpleOutputStream err(buffer, sizeof(buffer));

    for (auto testCase = testCases.begin(); testCase != testCases.begin() + testCount; ++testCase){
        testBuffer << testCase->name;

        // reset the buffer
        err.clear();

        bool result = testCase->testFunc(err);
        if (result) {
            testBuffer << "[PASS]" << std::endl;
            passedCount++;
        } else {
            testBuffer << "[FAIL]" << std::endl;
            testBuffer << err.str() << std::endl;
        }
    }

    testBuffer << std::endl << passedCount << "/" << totalCount << " tests passed." << std::endl;
    
    // Split the test results into lines
    std::istringstream iss(testBuffer.str());
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(iss, line)) {
        lines.push_back(line);
    }
    return lines;
}

// Function to display up to 20 lines from the buffer starting from a given line
void DisplayTestResults(int startLine, std::vector<std::string>& lines) {
    // Calculate the end line
    int endLine = startLine + 20;
    if (endLine > lines.size()) {
        endLine = lines.size();
    }

    // Clear the screen using ANSI escape codes
    console::clearConsole();

    // Print up to 20 lines starting from the specified line
    for (int i = startLine; i < endLine; ++i) {
        printf("%s\n", lines[i].c_str());
    }
}

int CountLines() {
    std::istringstream iss(testBuffer.str());
    std::string line;
    int lineCount = 0;
    while (std::getline(iss, line)) {
        lineCount++;
    }
    return lineCount;
}

void test_results_task(Gba_os::Task& t){
    scanKeys();
    u16 buttons = keysDown();
    static int display_line = 0;
    static const int line_count = CountLines();

    static bool rerender = true;

    if(buttons & KEY_UP){
        display_line++;
        if(display_line > line_count - 1){
            display_line = line_count - 1;
        }
        rerender = true;
    }
    if(buttons & KEY_DOWN){
        display_line--;
        if(display_line < 0){
            display_line = 0;
        }
        rerender = true;
    }
    if(rerender){
        std::vector<std::string>& lines = *(std::vector<std::string>*)t.data;
        DisplayTestResults(display_line, lines);
    }
    rerender = false;
}

} // namespace gba_os::test
