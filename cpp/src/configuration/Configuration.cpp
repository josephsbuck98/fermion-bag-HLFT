#include <stdexcept>
#include <cmath>
#include <string>

#include "Configuration.hpp"

//TODO: TOLERANCE CHECK when retrieving by key? Make a genKey or createKey func to take the float and get the correct key.
//TODO: If a tau is already present, do not allow a bond with that tau to be added
//TODO: Error/special case handling (for example, if getBond is called with a tau that doesn't exist.)
//TODO: Implement delBonds (with an s)

Configuration::Configuration(double tol) {
  tolerance = tol;
  bonds = {};
}

void Configuration::addBond(double tau, Bond& bond) {
  bonds.insert_or_assign(truncateToTolerance(tau), bond);
}

void Configuration::addBonds(std::vector<double> taus, std::vector<Bond> newBonds) {
  if (taus.size() != newBonds.size()) {
    throw std::runtime_error("Lengths of taus and newBonds must be the same, "
                  "but were " + std::to_string(taus.size()) + " and" 
                  " " + std::to_string(newBonds.size()));
  }
  for (int i = 0; i < newBonds.size(); i++) {
    addBond(taus[i], newBonds[i]);
  }
}

void Configuration::delBond(double tau) {
  //TODO: Handle deleting a bond that isn't there. May need try-catch if error gets thrown.
  bonds.erase(truncateToTolerance(tau));
}

const Bond& Configuration::getBond(double tau) const {
  auto it = bonds.find(truncateToTolerance(tau));
  if (it != bonds.end()) {
    return it->second;
  } //TODO: May want to change return type to Bond* so can return nullptr. That way, don't have to throw exception.
  throw std::out_of_range("Bond with specified tau not found"); 
}

const std::map<double, Bond>& Configuration::getBonds() const {
  return bonds;
}

int Configuration::getNumBonds() const {
  return bonds.size();
}

bool Configuration::operator==(const Configuration& other) const {
  if (this->getNumBonds() != other.getNumBonds()) {
    return false;
  }
  for (const auto& pair : bonds) {
    if (bonds.at(truncateToTolerance(pair.first)) != other.bonds.at(truncateToTolerance(pair.first))) {
      return false;
    }
  }
  return true;
}

bool Configuration::operator!=(const Configuration& other) const {
  return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Configuration& configuration) {
  std::map<double, Bond> bonds = configuration.getBonds();
  os << "Configuration:{\n";
  for (auto it = bonds.begin(); it != bonds.end(); ++it) {
    os << "Tau: " << it->first << ", " << it ->second;
  }
  os << "}\n\n";
  return os;
}

double Configuration::truncateToTolerance(double key) const {
  return std::round(key / tolerance) * tolerance;
}