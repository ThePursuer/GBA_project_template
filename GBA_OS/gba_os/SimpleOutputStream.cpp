// SimpleOutputStream.cpp
#include "gba_os/SimpleOutputStream.h"
#include <cstring>
#include <cstdio>

namespace Gba_os::console {

SimpleOutputStream::SimpleOutputStream(char* buf, size_t size) : buffer(buf), capacity(size), length(0) {
    if (capacity > 0) {
        buffer[0] = '\0'; // Ensure buffer is initially empty
    }
}

SimpleOutputStream& SimpleOutputStream::operator<<(const std::string& str) {
    size_t strLen = str.length();
    if (length + strLen < capacity) {
        std::strncpy(buffer + length, str.c_str(), strLen);
        length += strLen;
        buffer[length] = '\0'; // Ensure null-termination
    }
    return *this;
}

SimpleOutputStream& SimpleOutputStream::operator<<(const char* str) {
    size_t strLen = std::strlen(str);
    if (length + strLen < capacity) {
        std::strncpy(buffer + length, str, strLen);
        length += strLen;
        buffer[length] = '\0'; // Ensure null-termination
    }
    return *this;
}

SimpleOutputStream& SimpleOutputStream::operator<<(long long value) {
    constexpr size_t intMaxSize = 21 + 1; // Reserve space for maximum size of long long + null terminator
    if (length + intMaxSize < capacity) {
        int written = snprintf(buffer + length, capacity - length, "%lld", value);
        if (written > 0) {
            length += written;
        }
    }
    return *this;
}

SimpleOutputStream& SimpleOutputStream::operator<<(unsigned long long value) {
    constexpr size_t intMaxSize = 21 + 1; // Reserve space for maximum size of unsigned long long + null terminator
    if (length + intMaxSize < capacity) {
        int written = snprintf(buffer + length, capacity - length, "%llu", value);
        if (written > 0) {
            length += written;
        }
    }
    return *this;
}

SimpleOutputStream& SimpleOutputStream::operator<<(double value) {
    constexpr size_t doubleMaxSize = 24 + 1; // Reserve space for maximum size of double + null terminator
    if (length + doubleMaxSize < capacity) {
        int written = snprintf(buffer + length, capacity - length, "%f", value);
        if (written > 0) {
            length += written;
        }
    }
    return *this;
}

SimpleOutputStream& SimpleOutputStream::operator<<(std::ostream& (*manip)(std::ostream&)) {
    // Apply the manipulator to a temporary stringstream to capture its effect
    if (manip == static_cast<std::ostream& (*)(std::ostream&)>(std::endl)) {
        // If it's std::endl, just append a newline character
        // Ensure there's space for the newline and null terminator
        if (length + 1 < capacity) {
            buffer[length++] = '\n';
            buffer[length] = '\0'; // Ensure null-termination
        }
    }
    // For other manipulators, you can extend the logic here
    return *this;
}

void SimpleOutputStream::clear() {
    length = 0;
    buffer[0] = '\0';
}

std::string SimpleOutputStream::str() const {
    return std::string(buffer);
}

} // namespace Gba_os::console
