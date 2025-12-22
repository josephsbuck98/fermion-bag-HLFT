#pragma once

#include "LatticeBase.hpp"

#include <iostream>


class SimpleCubic : public LatticeBase {
public:
  struct Site {
    // xi, yi, and zi are integer coordinates of the atoms in the SimpleCubic 
    // lattice. 
    int xi, yi, zi;
    Site(int xii, int yii, int zii): xi(xii), yi(yii), zi(zii) {}
    friend std::ostream& operator<<(std::ostream& os, const Site& site) {
      os << std::to_string(site.xi) << ", " << std::to_string(site.yi) 
          << ", " << std::to_string(site.zi);
      return os;
    }
  };

  SimpleCubic(const LatticeInput& input);

  int getNumSites(consts::DirsType dir) const override;

  double getSite(consts::DirsType dir, int index) const;
  consts::BoundType getBoundType(consts::DirsType dir) const override;

  const Site& getSiteNEW(int xi, int yi, int zi) const;
  const std::vector<Site>& getSitesNEW() const;
  std::vector<Site> getNearestNeighborsNEW(const Site& site);
  int getTotNumSitesNEW() const;

  void printInfo() const override;

  
private:
  consts::DimsType dims;

  //TODO: Store attributes (xMin, ..., a, ..., xNSites, ...)

  std::vector<Site> sitesNEW; // Unlike sites, this contains integer locations, not absolute (double) locations
  int xNSites, yNSites, zNSites;
  double xMin, yMin, zMin;
  double a, b, c;
  double alpha, beta, gamma;
  std::vector<Site> createSimpleCubicNEW(const LatticeInput& input);






  std::unordered_map<consts::DirsType, std::vector<double>, 
      std::EnumClassHash<consts::DirsType>> sites;

  std::unordered_map<consts::DirsType, std::vector<double>, 
    std::EnumClassHash<consts::DirsType>> createSimpleCubic(LatticeInput input);

  std::vector<double> genUniform1DLattice(double min, double base, int nsites);
};