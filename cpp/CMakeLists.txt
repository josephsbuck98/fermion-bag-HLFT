cmake_minimum_required(VERSION 3.10)

project(fermion-bag-HLFT)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(yaml-cpp REQUIRED)

add_subdirectory(src)

target_link_libraries(your_target_name PRIVATE yaml-cpp)

