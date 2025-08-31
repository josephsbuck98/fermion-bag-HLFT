# src/hamiltonians/CMakeLists.txt

add_library(hamiltonians_lib Random.cpp TVModel.cpp)
target_include_directories(hamiltonians_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(hamiltonians_lib
  PUBLIC io_lib utils_lib
)
