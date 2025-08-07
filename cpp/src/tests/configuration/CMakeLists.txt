add_library(configuration_test_lib BondTests.cpp ConfigurationTests.cpp LatticeTests.cpp)
target_include_directories(configuration_test_lib PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/src/configuration
)
