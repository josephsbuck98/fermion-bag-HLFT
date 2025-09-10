# src/io/CMakeLists.txt

add_library(io_lib InputParser.cpp Output.cpp)
target_include_directories(io_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(io_lib 
  PUBLIC yaml-cpp utils_lib configuration_lib control_lib
)
