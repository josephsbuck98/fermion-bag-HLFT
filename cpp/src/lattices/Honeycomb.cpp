#include "Honeycomb.hpp"

#include <iostream>


Honeycomb::Honeycomb(int nCells_)
    : nCells(nCells_) {}

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