#include "Bond.hpp"

Bond::Bond(const std::set<int>& indices_) {
  indices = indices_;
  numSites = indices.size();
}

int Bond::getNumSites() const {
  return numSites;
};

const std::set<int>& Bond::getIndices() const {
  return indices;
};

bool Bond::operator==(const Bond& other) const {
  return indices == other.indices;
}

bool Bond::operator!=(const Bond& other) const {
  return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Bond& bond) {
  std::set<int> inds = bond.getIndices();
  for (auto it = inds.begin(); it != inds.end(); ++it) {
    os << *it << " ";
  }

  return os;
}