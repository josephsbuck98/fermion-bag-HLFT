#pragma once

#include "LatticeBase.hpp"

#include <iostream>


class SimpleCubic : public LatticeBase {
public:
  SimpleCubic(const LatticeInput& input);

  int getNumSites(consts::DirsType dir) const override;

  double getSite(consts::DirsType dir, int index) const;

  consts::BoundType getBoundType(consts::DirsType dir) const override;

  void printInfo() const override;

private:
  consts::DimsType dims;

  std::unordered_map<consts::DirsType, std::vector<double>, 
      std::EnumClassHash<consts::DirsType>> sites;

  std::unordered_map<consts::DirsType, std::vector<double>, 
    std::EnumClassHash<consts::DirsType>> createSimpleCubic(LatticeInput input);

  std::vector<double> genUniform1DLattice(double min, double base, int nsites);
};