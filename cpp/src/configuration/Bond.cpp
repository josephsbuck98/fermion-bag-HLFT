#include "Bond.hpp"

//TODO: Tolerance check when retrieving by key? Make a genKey or createKey func to take the float and get the correct key.

Bond::Bond(int numSites_, const std::vector<int>& indices_) 
  : numSites(numSites_), indices(indices_) {}

int Bond::getNumSites() const {
  return numSites;
};

const std::vector<int>& Bond::getIndices() const {
  return indices;
};
