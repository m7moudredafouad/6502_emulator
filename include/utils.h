#pragma once

#include <iostream>

#define ASSERT(expr, msg)                                                      \
    if (!expr) {                                                               \
        std::cout << "Assertion Error in " << __FILE__ << ":" << __LINE__      \
                  << " in function " << __func__ << "\n\t" << msg              \
                  << std::endl;                                                \
        exit(EXIT_FAILURE);                                                    \
    }
