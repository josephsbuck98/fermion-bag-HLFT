#pragma once

#include <map>
#include <vector>

#include "Bond.hpp"

class Configuration {
public:
  Configuration(double tol);

  void addBond(double tau, Bond& newBond);
  void addBonds(std::vector<double> taus, std::vector<Bond> newBonds);
  void delBond(double tau);
  void delBonds();

  const Bond& getBond(double tau) const;
  const std::map<double, Bond>& getBonds() const;

  int getNumBonds() const;

  bool operator==(const Configuration& other) const;
  bool operator!=(const Configuration& other) const;

  friend std::ostream& operator<<(std::ostream& os, const Configuration& configuration);

private:
  double truncateToTolerance(double key) const;

  double tolerance;
  std::map<double, Bond> bonds;

};