# src/lattices/CMakeLists.txt

add_library(lattices_lib SimpleCubic.cpp Honeycomb.cpp)
target_include_directories(lattices_lib PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/src/utils
)

target_link_libraries(lattices_lib
  PUBLIC io_lib utils_lib
)