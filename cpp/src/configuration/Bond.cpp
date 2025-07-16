#include "Bond.hpp"

Bond::Bond(const std::vector<int>& indices_) {
  indices = indices_;
  numSites = indices.size();
}

int Bond::getNumSites() const {
  return numSites;
};

const std::vector<int>& Bond::getIndices() const {
  return indices;
};
