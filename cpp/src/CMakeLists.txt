# src/CMakeLists.txt

# Define the main executable
add_executable(main main.cpp)

# Add subdirectories
add_subdirectory(algorithms)
add_subdirectory(models)
add_subdirectory(tests)
add_subdirectory(utils)

# Link with module libraries
target_link_libraries(main
  PRIVATE
    algorithms_lib
    models_lib
    tests_lib
    utils_lib
)

# Expose include directories for modules (optional)
target_include_directories(main
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/algorithms
    ${CMAKE_CURRENT_SOURCE_DIR}/models
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
)
