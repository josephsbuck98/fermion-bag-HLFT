add_library(io_test_lib InputTests.cpp)
target_include_directories(io_test_lib PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/src/io
)
target_link_libraries(io_test_lib 
  PRIVATE yaml-cpp utils_lib
)