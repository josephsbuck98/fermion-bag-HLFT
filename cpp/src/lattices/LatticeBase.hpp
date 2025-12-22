#pragma once

#include "Constants.hpp"
#include "Input.hpp"


class LatticeBase {
public:
  LatticeBase(const LatticeInput& input) : input(input), type(input.type) {
    boundTypes[consts::DirsType::X] = input.xBCType;
    boundTypes[consts::DirsType::Y] = input.yBCType;
    boundTypes[consts::DirsType::Z] = input.zBCType;
  };

  virtual ~LatticeBase() = default;

  virtual void printInfo() const = 0;

  virtual int getNumSites(consts::DirsType dir) const = 0;

  virtual consts::BoundType getBoundType(consts::DirsType dir) const = 0;

protected:
  LatticeInput input;

  consts::LatticeType type;

  std::unordered_map<consts::DirsType, consts::BoundType, 
    std::EnumClassHash<consts::DirsType>> boundTypes;

};