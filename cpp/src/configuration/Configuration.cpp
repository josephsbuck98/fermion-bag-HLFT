#include <stdexcept>
#include <cmath>
#include <string>

#include "Configuration.hpp"

//TODO: Error/special case handling (for example, if getBond is called with a tau that doesn't exist.)
//TODO: Implement delBonds (with an s)

std::vector<double> generateTauGroupStarts(double beta, int initNumTimeGroups);

Configuration::Configuration(ConfigurationInput input) {
  tolerance = input.float_tol;
  beta = input.beta;
  bonds = {};
  taus = {};
  maxNbondsPerGroup = input.maxNbondsPerGroup;
  tauGroupStarts = generateTauGroupStarts(beta, input.initNumTimeGroups);
}

std::vector<double> generateTauGroupStarts(double beta, int initNumTimeGroups) {
  double groupWidth = beta / initNumTimeGroups;
  std::vector<double> tauGroupStarts(initNumTimeGroups);
  tauGroupStarts[0] = 0.0;
  for (int i = 1; i < initNumTimeGroups; i++) {
    tauGroupStarts[i] = i * groupWidth;
  }
  return tauGroupStarts;
}

void Configuration::addBond(double tau, Bond& bond) {
  double tauTrunc = truncateToTolerance(tau);
  auto retSetPair = taus.insert(tauTrunc);
  auto retMapPair = bonds.insert({tauTrunc, bond});
  if (!retSetPair.second || !retMapPair.second) { 
    delBond(tau);
    throw std::runtime_error("Insert failed: Bond with tau" 
                  "=" + std::to_string(tau) + " already exists.");
  }
}

void Configuration::addBonds(std::vector<double> newTaus, std::vector<Bond> newBonds) {
  if (newTaus.size() != newBonds.size()) {
    throw std::runtime_error("Lengths of newTaus and newBonds must be the same, "
                  "but were " + std::to_string(newTaus.size()) + " and" 
                  " " + std::to_string(newBonds.size()));
  }
  for (int i = 0; i < newBonds.size(); i++) {
    try {
      addBond(newTaus[i], newBonds[i]);
    } catch (std::runtime_error err) {
      std::cout << err.what() << std::endl;
    }
  }
}

void Configuration::delBond(double tau) {
  double tauTrunc = truncateToTolerance(tau);
  size_t retSet = taus.erase(tauTrunc);
  size_t retMap = bonds.erase(tauTrunc);
  if (retSet == 0 || retMap == 0) {
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
  return taus.size();
}

bool Configuration::operator==(const Configuration& other) const {
  if (this->getNumBonds() != other.getNumBonds()) {
    return false;
  }
  for (const auto& pair : bonds) {
    double truncTol = pair.first;
    if (this->getBond(truncateToTolerance(truncTol)) 
          != other.getBond(truncateToTolerance(truncTol))) {      
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