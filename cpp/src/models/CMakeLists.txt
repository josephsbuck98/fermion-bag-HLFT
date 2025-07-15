# src/models/CMakeLists.txt

add_library(models_lib dummy.cpp)
target_include_directories(models_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
