cmake_minimum_required(VERSION 3.10)

project(fermion-bag-HLFT)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Eigen3 3.3 REQUIRED)
find_package(yaml-cpp REQUIRED)

add_subdirectory(src)

# Flags so you can debug with gdb -tui ./main
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
