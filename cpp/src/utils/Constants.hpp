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
  enum class HamilModel {
    RANDOM
  };
  inline const std::unordered_map<std::string, HamilModel> HAMIL_MODEL_MAP = {
    {"random", HamilModel::RANDOM}
  };

  enum class LatticeType {
    SIMPLE_CUBIC,
    HONEYCOMB
  };
  inline const std::unordered_map<std::string, LatticeType> LATTICE_TYPE_MAP = {
    {"simple-cubic", LatticeType::SIMPLE_CUBIC},
    {"honeycomb", LatticeType::HONEYCOMB}
  };

  enum class BoundType {
    OPEN,
    PERIODIC
  };
  inline const std::unordered_map<std::string, BoundType> BOUND_TYPE_MAP = {
    {"open", BoundType::OPEN},
    {"periodic", BoundType::PERIODIC}
  };

}