#pragma once

#include <map>
#include <set>
#include <vector>

#include "Bond.hpp"
#include "Input.hpp"

class Configuration {
public:
  Configuration() = default;
  Configuration(ConfigurationInput input);

  void setTauGroupStarts(std::vector<double> newTauGroupStarts);
  const std::vector<double>& getTauGroupStarts() const;

  std::set<std::pair<double, int>> getTaus() const;

  double getTolerance() const;

  int getAvgNbondsPerGroup() const;

  int calcNumTimeGroups(int initNumTimeGroups);

  void addBond(std::pair<double, int> tau, Bond& newBond);
  void addBonds(std::vector<std::pair<double, int>> newTaus, std::vector<Bond> newBonds);
  void delBond(std::pair<double, int> tau);
  void delBonds();

  const Bond& getBond(double tau) const;
  const std::map<double, Bond>& getBonds() const;
  int getNumBonds() const;
  std::map<int, int> getBondsPerType() const;

  double getBeta() const;

  bool operator==(const Configuration& other) const;
  bool operator!=(const Configuration& other) const;

  friend std::ostream& operator<<(std::ostream& os, const Configuration& configuration);
  friend std::istream& operator>>(std::istream& is, const Configuration& configuration);

private:
  double truncateToTolerance(double key) const;

  double tolerance; 
  double beta;

  std::map<double, Bond> bonds;
  // std::set<double> taus;
  std::set<std::pair<double, int>> taus;

  int avgNbondsPerGroup;
  std::vector<double> tauGroupStarts;
  std::map<int, int> bondsPerType;

};


std::vector<double> generateTauGroupStarts(double beta, int initNumTimeGroups);
