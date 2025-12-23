#pragma once

#include "LatticeBase.hpp"

#include <iostream>


class SimpleCubic : public LatticeBase {
public:
  struct Site : public SiteBase {
    // xi, yi, and zi are integer coordinates of the atoms in the SimpleCubic 
    // lattice. 

    Site(int xi, int yi, int zi) : SiteBase(xi, yi, zi) {};
    
    //DELETE ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // int xi, yi, zi;
    // Site(int xii, int yii, int zii): xi(xii), yi(yii), zi(zii) {}
    // friend std::ostream& operator<<(std::ostream& os, const Site& site) {
    //   os << std::to_string(site.xi) << ", " << std::to_string(site.yi) 
    //       << ", " << std::to_string(site.zi);
    //   return os;
    // }
  };

  SimpleCubic(const LatticeInput& input);

  int getNumSites(consts::DirsType dir) const override;

  //DELETE ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  // double getSite(consts::DirsType dir, int index) const;
  
  consts::BoundType getBoundType(consts::DirsType dir) const override;

  const Site& getSite(int xi, int yi, int zi) const;
  const std::vector<Site>& getSites() const;
  std::vector<Site> getNearestNeighbors(const Site& site);
  int getTotNumSites() const;

  void printInfo() const override;

  
private:
  consts::DimsType dims;

  //TODO: Store attributes (xMin, ..., a, ..., xNSites, ...)

  std::vector<Site> sites; // Unlike sites, this contains integer locations, not absolute (double) locations
  int xNSites, yNSites, zNSites;
  double xMin, yMin, zMin;
  double a, b, c;
  double alpha, beta, gamma;
  std::vector<Site> createSimpleCubic(const LatticeInput& input);






  //DELETE ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  // std::unordered_map<consts::DirsType, std::vector<double>, 
  //     std::EnumClassHash<consts::DirsType>> sites;

  //DELETE ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  // std::unordered_map<consts::DirsType, std::vector<double>, 
  //   std::EnumClassHash<consts::DirsType>> createSimpleCubic(LatticeInput input);

  //DELETE ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  // std::vector<double> genUniform1DLattice(double min, double base, int nsites);
};