#pragma once

#include <iostream>
#include <map>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Constants.hpp"
#include "Input.hpp"

class Lattice {
public:
  
  Lattice(LatticeInput input);

  int getNumSites(consts::DirsType dir) const;
  
  double getSite(consts::DirsType dir, int index) const; 

private:
  consts::LatticeType type;
  consts::DimsType dims;
  std::unordered_map<consts::DirsType, std::vector<double>, 
      std::EnumClassHash<consts::DirsType>> sites;

  std::unordered_map<consts::DirsType, std::vector<double>, 
    std::EnumClassHash<consts::DirsType>> createSimpleCubic(LatticeInput input);
};
