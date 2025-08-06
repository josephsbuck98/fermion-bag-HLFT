# src/io/CMakeLists.txt

add_library(io_lib InputParser.cpp)
target_include_directories(io_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
