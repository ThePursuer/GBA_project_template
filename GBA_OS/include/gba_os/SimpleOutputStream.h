// SimpleOutputStream.h
#ifndef SIMPLE_OUTPUT_STREAM_H
#define SIMPLE_OUTPUT_STREAM_H

#include <string>
#include <cstddef> // for size_t
#include <cstring>
#include <cstdio>
#include <ostream>

namespace Gba_os::console {


/**
 * @name SimpleOutputStream
 * @brief A simple output stream class that can be used to build strings
 * ostream and stringstream are absurdly large, over 150 bytes for some reason.
 * This implementation is 12 bytes.
 * 
 * To create an instance of this class, you need to provide a buffer and its size.
*/
class SimpleOutputStream {
private:
    char* buffer;
    size_t capacity;
    size_t length;

public:
    SimpleOutputStream(char* buf, size_t size);

    SimpleOutputStream& operator<<(const char* str);
    SimpleOutputStream& operator<<(const std::string& str);
    
    SimpleOutputStream& operator<<(long long value);
    SimpleOutputStream& operator<<(unsigned long long value);
    SimpleOutputStream& operator<<(short value){ return *this << (long long)value; }
    SimpleOutputStream& operator<<(unsigned short value){ return *this << (unsigned long long)value; }
    SimpleOutputStream& operator<<(int value){ return *this << (long long)value; }
    SimpleOutputStream& operator<<(unsigned int value){ return *this << (unsigned long long)value; }

    SimpleOutputStream& operator<<(double value);
    SimpleOutputStream& operator<<(float value){ return *this << (double)value; }
    SimpleOutputStream& operator<<(std::ostream& (*manip)(std::ostream&));

    void clear();
    std::string str() const;
};

} // namespace Gba_os::console

#endif // SIMPLE_OUTPUT_STREAM_H
