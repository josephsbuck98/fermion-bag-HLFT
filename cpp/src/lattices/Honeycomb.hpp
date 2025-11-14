#pragma once


#include "LatticeBase.hpp"

#include "Constants.hpp"
#include "Input.hpp"

#include <iostream>


class Honeycomb : public LatticeBase {
public:
  explicit Honeycomb(int nCells);

  int getNumSites(consts::DirsType dir) const override;

  consts::BoundType getBoundType(consts::DirsType dir) const override;

  void printInfo() const override;

private:
  int nCells;

  std::unordered_map<consts::DirsType, consts::BoundType, 
    std::EnumClassHash<consts::DirsType>> boundTypes;
};