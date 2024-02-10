#include <iostream>
#include <cstdint>

#include "division_table.h" // assumes you have generated your file correctly


int main(int argc, char* argv[]) {
    if(argc != 2) {
        std::cout << "Usage: " << argv[0] << " PRECISION" << std::endl;
        return 1;
    }
    int precision = std::stoi(argv[1]);

    for (int i = 0; i < sizeof(division_table) / sizeof(division_table[0]); i++) {
        std::cout << "Magic number " << i << ": " << std::to_string(division_table[i]) << std::endl;
        std::cout << "Test 20 / " << i + 1 << " = " << ((((20 << precision) * division_table[i]) >> precision) >> precision) << std::endl;
        std::cout << "Test 20 / " << i + 1 << " = " << (((20 << precision) * division_table[i]) >> precision) << std::endl << std::endl;
    }

    return 0;
}
