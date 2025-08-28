# src/control/CMakeLists.txt

add_library(control_lib Driver.cpp Sweep.cpp Update.cpp)
target_include_directories(control_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(control_lib 
  PUBLIC configuration_lib io_lib utils_lib
)
