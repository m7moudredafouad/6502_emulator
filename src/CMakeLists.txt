file(GLOB SRC_FILES *.cpp)

add_library(6502_lib ${SRC_FILES})
target_include_directories(6502_lib PRIVATE ../include/)