#pragma once

#include <map>
#include <set>
#include <vector>

#include "Bond.hpp"
#include "Input.hpp"

class Configuration {
public:
  Configuration(ConfigurationInput input);

  void setTauGroupStarts(std::vector<double> newTauGroupStarts);

  std::set<double> getTaus() const;

  int getAvgNbondsPerGroup() const;

  int calcNumTimeGroups(int initNumTimeGroups);

  void addBond(double tau, Bond& newBond);
  void addBonds(std::vector<double> taus, std::vector<Bond> newBonds);
  void delBond(double tau);
  void delBonds();

  const Bond& getBond(double tau) const;
  const std::map<double, Bond>& getBonds() const;
  int getNumBonds() const;

  double getBeta() const;

  bool operator==(const Configuration& other) const;
  bool operator!=(const Configuration& other) const;

  friend std::ostream& operator<<(std::ostream& os, const Configuration& configuration);

private:
  double truncateToTolerance(double key) const;

  double tolerance; 
  double beta;
  std::map<double, Bond> bonds;
  std::set<double> taus;
  int avgNbondsPerGroup;
  std::vector<double> tauGroupStarts;

};


std::vector<double> generateTauGroupStarts(double beta, int initNumTimeGroups);
