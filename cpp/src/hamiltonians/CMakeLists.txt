# src/hamiltonians/CMakeLists.txt

add_library(hamiltonians_lib dummy.cpp)
target_include_directories(hamiltonians_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
