# src/configuration/CMakeLists.txt

add_library(configuration_lib Configuration.cpp Bond.cpp Lattice.cpp)
target_include_directories(configuration_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})