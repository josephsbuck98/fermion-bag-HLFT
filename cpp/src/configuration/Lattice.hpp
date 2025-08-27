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

  int getNumSites(consts::DirsType dir);
  
  double getSite(consts::DirsType dir, int index) const; 

  //TODO: Revise to use enum names. WILL NEED TO REVISE ALL LATTICE UNIT TESTS
  //TODO: Use input as input to prepConstInputs
  static std::pair<std::map<std::string, std::pair<double, double>>, 
      std::map<std::string, int>> prepConstInputs(LatticeInput latInput);

private:
  consts::LatticeType type;
  consts::DimsType dims;
  std::unordered_map<consts::DirsType, std::vector<double>, 
      std::EnumClassHash<consts::DirsType>> sites;

  std::unordered_map<consts::DirsType, std::vector<double>, 
    std::EnumClassHash<consts::DirsType>> createSimpleCubic(LatticeInput input);
};
