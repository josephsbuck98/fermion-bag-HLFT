#include "Bond.hpp"


Bond::Bond(const std::set<const SiteBase*>& sites_) 
    : sites(sites_.begin(), sites_.end()) {
  numSites = sites.size();
}

int Bond::getNumSites() const {
  return numSites;
};

const std::set<const SiteBase*, Bond::SiteSumLess>& Bond::getSites() const {
  return sites;
}

bool Bond::operator==(const Bond& other) const {
  return sites == other.sites;
}

bool Bond::operator!=(const Bond& other) const {
  return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Bond& bond) {
  for (const auto* s : bond.sites) {os << *s;}
  return os;
}