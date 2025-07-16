#include <stdexcept>

#include "Configuration.hpp"

//TODO: Tolerance check when retrieving by key? Make a genKey or createKey func to take the float and get the correct key.
//TODO: If a tau is already present, do not allow a bond with that tau to be added

void Configuration::addBond(double tau, Bond& bond) {
  bonds.insert_or_assign(tau, bond);
}

void Configuration::delBond(double tau) {
  //TODO: Handle deleting a bond that isn't there
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