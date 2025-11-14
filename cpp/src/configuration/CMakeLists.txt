# src/configuration/CMakeLists.txt

add_library(configuration_lib Configuration.cpp Bond.cpp)
target_include_directories(configuration_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(configuration_lib 
  PUBLIC utils_lib io_lib
)
