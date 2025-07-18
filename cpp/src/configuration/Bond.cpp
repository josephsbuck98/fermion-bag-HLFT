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
  os << "Bond:\n\tNumSites=" << bond.numSites;
  os << "\n\tIndices={";
  for (auto it = bond.indices.begin(); it != bond.indices.end(); ++it) {
    os << *it;
    if (std::next(it) != bond.indices.end()) os << ", ";
  }
  os << "}\n";
  return os;
}