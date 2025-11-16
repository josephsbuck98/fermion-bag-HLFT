# src/hamiltonians/CMakeLists.txt

add_library(hamiltonians_lib Random.cpp TVModel.cpp HamiltonianBase.cpp RandomNEW.cpp TVModelNEW.cpp)
target_include_directories(hamiltonians_lib PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/src/configuration
  ${CMAKE_SOURCE_DIR}/src/utils
)
target_link_libraries(hamiltonians_lib
  PUBLIC io_lib lattices_lib utils_lib
)
