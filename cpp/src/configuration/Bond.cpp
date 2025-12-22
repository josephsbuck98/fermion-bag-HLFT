#include "Bond.hpp"

Bond::Bond(const std::set<int>& indices_) {
  indices = indices_;
  numSites = indices.size();
}

Bond::Bond(const std::set<const Site*>& sites_) {
  sites = sites_;
  numSites = sites.size();
}

int Bond::getNumSites() const {
  return numSites;
};

const std::set<int>& Bond::getIndices() const {
  return indices;
};

bool Bond::operator==(const Bond& other) const {
  return indices == other.indices; //TODO: Replace with sites comparison
}

bool Bond::operator!=(const Bond& other) const {
  return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Bond& bond) {
  const std::set<int>& inds = bond.getIndices();
  for (auto it = inds.begin(); it != inds.end(); ++it) {
    os << *it << " ";
  } //TODO: Replace with output of sites

  return os;
}