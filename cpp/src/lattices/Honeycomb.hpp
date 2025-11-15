#pragma once

#include "LatticeBase.hpp"

#include <iostream>


class Honeycomb : public LatticeBase {
public:
  explicit Honeycomb(const LatticeInput& input);

  int getNumSites(consts::DirsType dir) const override;

  consts::BoundType getBoundType(consts::DirsType dir) const override;

  void printInfo() const override;

private:
  int nCells;
  
};