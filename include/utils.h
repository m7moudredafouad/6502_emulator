#pragma once

#include <iostream>

#define ASSERT(expr, msg)                                                      \
    if (!expr) {                                                               \
        std::cout << "Assertion Error in " << __FILE__ << ":" << __LINE__      \
                  << " in function " << __func__ << "\n\t" << msg              \
                  << std::endl;                                                \
        exit(EXIT_FAILURE);                                                    \
    }

inline uint16_t address_from_bytes(uint8_t low, uint8_t high) {
    uint16_t address = high;
    address = address << 8;
    return address | low;
}

inline std::pair<uint8_t, uint8_t> bytes_from_address(uint16_t addr) {
    uint8_t low = addr & 0xFF;
    uint8_t high = (addr >> 8) & 0xFF;
    return std::make_pair(low, high);
}
