#pragma once

#include <Eigen/Dense>
#include <map>
#include <set>
#include <vector>

#include "Bond.hpp"
#include "Input.hpp"

class Configuration {
public:
  Configuration() = default;
  Configuration(ConfigurationInput input);

  void validate() const;

  void setTauGroupStarts(std::vector<double> newTauGroupStarts);
  const std::vector<double>& getTauGroupStarts() const;

  const std::set<std::pair<double, int>>& getTaus() const;

  bool setTolerance(double tol);
  double getTolerance() const;

  int calcNumTimeGroups(int numTimeGroups);

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
    std::set<std::pair<double, int>>::iterator itLow;
    std::set<std::pair<double, int>>::iterator itHigh;
    size_t nBondsInRegion;

    void computeIterators(const std::set<std::pair<double, int>>& taus) {
      //TODO: Come up with a way to detect if itLow and itHigh are undefined.
      itLow = taus.lower_bound({lower, std::numeric_limits<int>::min()});
      itHigh = taus.upper_bound({upper, std::numeric_limits<int>::max()});
      nBondsInRegion = std::distance(itLow, itHigh);
    }

    std::size_t size() const {
      return std::distance(itLow, itHigh);
    }
  };

  Eigen::MatrixXd getHProd(double omega, double cosh2alpha, 
      double sinh2alpha, double tau, const Bond& bond) const;
  void multToHProd(Eigen::MatrixXd& hProdMat, const Bond& bond, 
      double cosh2alpha, double sinh2alpha) const;
  Eigen::MatrixXd genMatForBond(int numSites, 
      std::vector<const SiteBase*>& bondSitesVec, double cosh2alpha, 
      double sinh2alpha) const;

private:
  double truncateToTolerance(double key) const;

  double tolerance; 
  double beta;

  std::map<double, Bond> bonds;
  std::set<std::pair<double, int>> taus;

  std::vector<double> tauGroupStarts;
  std::map<int, int> bondsPerType;

};


std::vector<double> generateTauGroupStarts(double beta, int numTimeGroups);
