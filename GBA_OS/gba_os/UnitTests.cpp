#include "gba_os/UnitTest.h"

#include "gba_os/Console.h"

namespace gba_os::test {

// Global vector to store all registered test cases
std::vector<TestCase> testCases;

// Buffer to capture test results
std::ostringstream testBuffer;

// Function to register a test case
void RegisterTest(const std::string& name, std::function<bool(std::ostringstream&)> testFunc) {
    testCases.push_back({name, testFunc, std::ostringstream()});
}

// Function to run all registered test cases and capture results in the buffer
void RunTests() {
    int passedCount = 0;
    int totalCount = testCases.size();

    for (TestCase& testCase : testCases) {
        testBuffer << testCase.name;

        bool result = testCase.testFunc(testCase.err);
        if (result) {
            testBuffer << "[PASS]" << std::endl;
            passedCount++;
        } else {
            testBuffer << "[FAIL]" << std::endl;
            testBuffer << testCase.err.str() << std::endl;
        }
    }

    testBuffer << std::endl << passedCount << "/" << totalCount << " tests passed." << std::endl;
}

// Function to display up to 20 lines from the buffer starting from a given line
void DisplayTestResults(int startLine) {
    // Split the test results into lines
    std::istringstream iss(testBuffer.str());
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(iss, line)) {
        lines.push_back(line);
    }

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

void test_results_task(gba_os::Task& t){
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
    if(rerender)
        DisplayTestResults(display_line);
    rerender = false;
}

} // namespace gba_os::test
