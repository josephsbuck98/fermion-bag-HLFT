# src/tests

# Add the test executable
add_executable(tests_main 
  tests_main.cpp
)

# Add subdirectories
add_subdirectory(algorithms)
add_subdirectory(configuration)
add_subdirectory(models)
add_subdirectory(utils)

# Link test libraries
target_link_libraries(tests_main
  PRIVATE
    algorithms_test_lib
    configuration_test_lib
    models_test_lib
    utils_test_lib
    algorithms_lib
    configuration_lib
    models_lib
    utils_lib
)

target_include_directories(tests_main PRIVATE
  ${CMAKE_SOURCE_DIR}/src/algorithms
  ${CMAKE_SOURCE_DIR}/src/configuration
  ${CMAKE_SOURCE_DIR}/src/models
  ${CMAKE_SOURCE_DIR}/src/utils
)
