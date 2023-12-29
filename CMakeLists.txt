cmake_minimum_required(VERSION 3.2)

project(6502_emulator)

set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(src/)
add_subdirectory(test/)

file(GLOB SRC_FILES *.cpp)

add_executable(6502_emulator ${SRC_FILES})
target_include_directories(6502_emulator PRIVATE include/)
target_link_libraries(6502_emulator PRIVATE 6502_lib)


