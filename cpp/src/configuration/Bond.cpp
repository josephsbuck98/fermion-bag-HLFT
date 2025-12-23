#include "Bond.hpp"

//DELETE ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Bond::Bond(const std::set<int>& indices_) {
//   indices = indices_;
//   numSites = indices.size();
// }



Bond::Bond(const std::set<const Site*>& sites_) {
  sites = sites_;
  numSites = sites.size();
}

int Bond::getNumSites() const {
  return numSites;
};

//DELETE ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// const std::set<int>& Bond::getIndices() const {
//   return indices;
// };

//DELETE ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// bool Bond::operator==(const Bond& other) const {
//   return indices == other.indices;

bool Bond::operator==(const Bond& other) const {
  return sites == other.sites;
}

bool Bond::operator!=(const Bond& other) const {
  return !(*this == other);
}

//DELETE ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// }td::ostream& operator<<(std::ostream& os, const Bond& bond) {
//   const std::set<int>& inds = bond.getIndices();
//   for (auto it = inds.begin(); it != inds.end(); ++it) {
//     os << *it << " ";
//   } //TODO: Replace with output of sites

//   return os;
// }

std::ostream& operator<<(std::ostream& os, const Bond& bond) {
  for (const auto* s : bond.sites) {
    os << s->xi << " " << s->yi << " " << s->zi " ";
  }
  return os;
}