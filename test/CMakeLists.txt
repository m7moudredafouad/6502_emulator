file(GLOB SRC_FILES *.cpp)

add_executable(6502_test ${SRC_FILES})
target_include_directories(6502_test PRIVATE ../include/)
target_link_libraries(6502_test PRIVATE 6502_lib gtest)