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

bool Bond::operator==(const Bond& other) const {
  if (numSites != other.numSites) {
    return false;
  }
  for (int i = 0; i < numSites; i++) {
    if (indices[i] != other.indices[i]) {
      return false;
    }
  }
  return true;
}

bool Bond::operator!=(const Bond& other) const {
  return !(*this == other);
}