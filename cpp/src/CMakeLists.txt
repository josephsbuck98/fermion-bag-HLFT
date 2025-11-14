# src/CMakeLists.txt

# Define the main executable
add_executable(main main.cpp)

# Add subdirectories
add_subdirectory(configuration)
add_subdirectory(control)
add_subdirectory(hamiltonians)
add_subdirectory(io)
add_subdirectory(lattices)
# add_subdirectory(tests)
add_subdirectory(utils)

# Link with module libraries
target_link_libraries(main
  PRIVATE
    configuration_lib
    control_lib
    hamiltonians_lib
    io_lib
    lattices_lib
    utils_lib
    yaml-cpp
)

# Expose include directories for modules (optional)
target_include_directories(main
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/configuration
    ${CMAKE_CURRENT_SOURCE_DIR}/control
    ${CMAKE_CURRENT_SOURCE_DIR}/hamiltonians
    ${CMAKE_CURRENT_SOURCE_DIR}/io
    ${CMAKE_CURRENT_SOURCE_DIR}/lattices
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
)
