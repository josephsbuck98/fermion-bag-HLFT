# src/control/CMakeLists.txt

add_library(control_lib Driver.cpp Sweep.cpp)
target_include_directories(control_lib PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/src/hamiltonians
)
target_link_libraries(control_lib 
  PUBLIC configuration_lib hamiltonians_lib io_lib lattices_lib utils_lib
)
