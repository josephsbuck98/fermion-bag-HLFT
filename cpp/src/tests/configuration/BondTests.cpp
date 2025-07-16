#include <cassert>
#include <iostream>

#include "Bond.hpp"

void runBondTests() {
  int numSites;
  std::vector<int> indices;
  
  std::cout << "Running BondTests...\n";
  
  numSites = 2;
  indices = {2, 3};
  Bond b(indices);
  
  testGetNumSites(numSites, b);
  testGetIndices(indices, b);
  
  std::cout << "Completed BondTests.\n";
};

void testGetNumSites(int numSites, Bond b) {
  assert(b.getNumSites() == numSites);
};

void testGetIndices(std::vector<int> indices, Bond b) {
  std::vector<int> bIndices = b.getIndices();
  for(int i = 0; i < bIndices.size(); i++) {
    assert(bIndices[i] == indices[i]);
  }
};