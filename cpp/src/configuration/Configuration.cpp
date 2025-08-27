#include <stdexcept>
#include <cmath>
#include <string>

#include "Configuration.hpp"

//TODO: Error/special case handling (for example, if getBond is called with a tau that doesn't exist.)
//TODO: Implement delBonds (with an s)

// Configuration::Configuration(double tol) {
//   tolerance = tol;
//   bonds = {};
// }

std::vector<double> generateTauGroupStarts(double beta, int num_time_groups_init);

Configuration::Configuration(ConfigurationInput input) {
  tolerance = input.float_tol;
  beta = input.beta;
  bonds = {};
  maxNbondsPerGroup = input.maxNbondsPerGroup;
  tauGroupStarts = generateTauGroupStarts(beta, input.num_time_groups_init);
}

std::vector<double> generateTauGroupStarts(double beta, int num_time_groups_init) {
  double groupWidth = beta / num_time_groups_init;
  std::vector<double> tauGroupStarts(num_time_groups_init);
  tauGroupStarts[0] = 0.0;
  for (int i = 1; i < num_time_groups_init; i++) {
    tauGroupStarts[i] = i * groupWidth;
  }
  return tauGroupStarts;
}

void Configuration::addBond(double tau, Bond& bond) {
  auto retPair = bonds.insert({truncateToTolerance(tau), bond});
  if (!retPair.second) {
    throw std::runtime_error("Insert failed: Bond with tau" 
                  "=" + std::to_string(tau) + " already exists.");
  }
}

void Configuration::addBonds(std::vector<double> taus, std::vector<Bond> newBonds) {
  if (taus.size() != newBonds.size()) {
    throw std::runtime_error("Lengths of taus and newBonds must be the same, "
                  "but were " + std::to_string(taus.size()) + " and" 
                  " " + std::to_string(newBonds.size()));
  }
  for (int i = 0; i < newBonds.size(); i++) {
    try {
      addBond(taus[i], newBonds[i]);
    } catch (std::runtime_error err) {
      std::cout << err.what() << std::endl;
    }
  }
}

void Configuration::delBond(double tau) {
  size_t ret = bonds.erase(truncateToTolerance(tau));
  if (ret == 0) {
    std::runtime_error err("Cannot delete element with tau"
              "=" + std::to_string(tau) + ". Element does not exist.");
    std::cout << err.what() << std::endl;
  }
}

void Configuration::delBonds() {
  bonds = {};
}

const Bond& Configuration::getBond(double tau) const {
  auto it = bonds.find(truncateToTolerance(tau));
  if (it != bonds.end()) {
    return it->second;
  } 
  static const Bond nullBond({});
  return nullBond;
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
    if (this->getBond(truncateToTolerance(pair.first)) 
          != other.getBond(truncateToTolerance(pair.first))) {      
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