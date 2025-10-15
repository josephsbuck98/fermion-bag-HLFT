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

  const std::set<std::pair<double, int>>& getTaus() const;

  bool setTolerance(double tol);
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

  bool setBeta(double b);
  double getBeta() const;

  bool operator==(const Configuration& other) const;
  bool operator!=(const Configuration& other) const;

  friend std::ostream& operator<<(std::ostream& os, const Configuration& configuration);
  friend std::istream& operator>>(std::istream& is, Configuration& configuration);

  // Struct to aid in determining the probabilities of accepting proposals
  struct RegionData {
    double lower; double upper;
    std::set<std::pair<double, int>>::iterator it_low;
    std::set<std::pair<double, int>>::iterator it_high;
    size_t nBondsInRegion;

    void computeIterators(const std::set<std::pair<double, int>>& taus) {
      it_low = taus.lower_bound({lower, std::numeric_limits<int>::min()});
      it_high = taus.upper_bound({upper, std::numeric_limits<int>::max()});
      nBondsInRegion = std::distance(it_low, it_high);
    }

    std::size_t size() const {
      return std::distance(it_low, it_high);
    }
  };

private:
  double truncateToTolerance(double key) const;

  double tolerance; 
  double beta;

  std::map<double, Bond> bonds;
  std::set<std::pair<double, int>> taus;

  int avgNbondsPerGroup;
  std::vector<double> tauGroupStarts;
  std::map<int, int> bondsPerType;

};


std::vector<double> generateTauGroupStarts(double beta, int initNumTimeGroups);
