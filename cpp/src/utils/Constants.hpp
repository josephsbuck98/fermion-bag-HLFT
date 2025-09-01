#pragma once

#include <cmath>
#include <string>
#include <unordered_map>


namespace consts {
  // Mathematical consts
  inline constexpr double pi = 3.14159265358979323846;

  // Physical consts
  inline constexpr double k_B    = 1.380649e-23;        // Boltzmann constant, J/K
  
  // Input consts
  enum class HamilModel {
    RANDOM, TVModel
  };
  inline const std::unordered_map<std::string, HamilModel> HAMIL_MODEL_MAP = {
    {"random", HamilModel::RANDOM},
    {"tV", HamilModel::TVModel} //TODO: Case insensitive?
  };

  enum class LatticeType {
    SIMPLE_CUBIC, HONEYCOMB
  };
  inline const std::unordered_map<std::string, LatticeType> LATTICE_TYPE_MAP = {
    {"simple-cubic", LatticeType::SIMPLE_CUBIC},
    {"honeycomb", LatticeType::HONEYCOMB}
  };

  enum class BoundType {
    OPEN, PERIODIC
  };
  inline const std::unordered_map<std::string, BoundType> BOUND_TYPE_MAP = {
    {"open", BoundType::OPEN},
    {"periodic", BoundType::PERIODIC}
  };

  enum class DimsType {
    ONE, TWO, THREE
  };
  inline const std::unordered_map<int, DimsType> DIMS_TYPE_MAP = {
    {1, DimsType::ONE},
    {2, DimsType::TWO},
    {3, DimsType::THREE}
  };

  enum class DirsType {
    X, Y, Z
  };
  inline const std::unordered_map<std::string, DirsType> DIRS_TYPE_MAP = {
    {"x", DirsType::X},
    {"y", DirsType::Y},
    {"z", DirsType::Z}
  };

  enum class BondActionType {
    REJECTION, INSERTION, REMOVAL
  };

}

namespace std {
  template <typename E>
  struct EnumClassHash {
    static_assert(std::is_enum_v<E>, "E must be an enum");
    std::size_t operator()(E e) const noexcept {
      return static_cast<std::size_t>(e);
    }
  };
}