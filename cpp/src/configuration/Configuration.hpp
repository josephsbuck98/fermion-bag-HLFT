#pragma once

#include <map>

#include "Bond.hpp"

class Configuration {
public:
  void addBond(double tau, Bond& newBond);
  void delBond(double tau);

  const Bond& getBond(double tau) const;
  const std::map<double, Bond>& getBonds() const;

  int getNumBonds() const;

  bool operator==(const Configuration& other) const;
  bool operator!=(const Configuration& other) const;

private:
  std::map<double, Bond> bonds;
};