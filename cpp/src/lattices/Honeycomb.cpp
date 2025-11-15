#include "Honeycomb.hpp"

#include <iostream>


Honeycomb::Honeycomb(const LatticeInput& input)
    : nCells(99999), LatticeBase(input) {} //TODO: Implement Honeycomb constructor

void Honeycomb::printInfo() const {
    std::cout << "Honeycomb Lattice with " 
              << nCells << " cells\n";
}

int Honeycomb::getNumSites(consts::DirsType dir) const {
  return 0;
}

consts::BoundType Honeycomb::getBoundType(consts::DirsType dir) const {
  return boundTypes.at(dir);
}