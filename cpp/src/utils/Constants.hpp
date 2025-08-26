#pragma once
#include <cmath>
#include <string>
#include <unordered_set>

namespace constants {
  // Mathematical constants
  inline constexpr double pi = 3.14159265358979323846;

  // Physical constants
  inline constexpr double k_B    = 1.380649e-23;        // Boltzmann constant, J/K
  
  // Input constants
  inline const std::unordered_set<std::string> ALLOWED_HAMIL_MODELS = {
    "random"
  };
  inline const std::unordered_set<std::string> ALLOWED_LATTICE_TYPES = {
    "simple-cubic", "honeycomb"
  };
  inline const std::unordered_set<std::string> ALLOWED_BOUNDARY_TYPES = {
    "open", "periodic"
  };

}