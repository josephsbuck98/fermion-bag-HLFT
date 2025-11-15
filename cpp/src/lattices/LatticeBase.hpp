#pragma once

#include "Constants.hpp"
#include "Input.hpp"


class LatticeBase {
public:
  LatticeBase(const LatticeInput& input) : type(input.type) {
    boundTypes[consts::DirsType::X] = input.x_bc_type;
  };

  virtual ~LatticeBase() = default;

  virtual void printInfo() const = 0;

  virtual int getNumSites(consts::DirsType dir) const = 0;

  virtual consts::BoundType getBoundType(consts::DirsType dir) const = 0;

protected:
  consts::LatticeType type;

  std::unordered_map<consts::DirsType, consts::BoundType, 
    std::EnumClassHash<consts::DirsType>> boundTypes;

};