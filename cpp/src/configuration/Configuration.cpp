#include <stdexcept>

#include "Configuration.hpp"

void Configuration::addBond(double tau, Bond& bond) {
  bonds.insert_or_assign(tau, bond);
}

void Configuration::delBond(double tau) {
  bonds.erase(tau);
}

const Bond& Configuration::getBond(double tau) const {
  auto it = bonds.find(tau);
  if (it != bonds.end()) {
    return it->second;
  }
  throw std::out_of_range("Bond with specified tau not found");
}

const std::map<double, Bond>& Configuration::getBonds() const {
  return bonds;
}

int Configuration::getNumBonds() {
  return bonds.size();
}