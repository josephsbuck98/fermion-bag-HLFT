#pragma once

#include "LatticeBase.hpp"

#include <iostream>


class SimpleCubic : public LatticeBase {
public:
  struct Site : public SiteBase {
    // xi, yi, and zi are integer coordinates of the atoms in the SimpleCubic 
    // lattice. 
    Site(int xi, int yi, int zi) : SiteBase(xi, yi, zi) {};
    
    //TODO: Implement the << operator
  };

  SimpleCubic(const LatticeInput& input);

  int getNumSites(consts::DirsType dir) const override;
  
  consts::BoundType getBoundType(consts::DirsType dir) const override;

  int getSiteInd(int xi, int yi, int zi) const override;
  const Site& getSite(int xi, int yi, int zi) const override;
  const std::vector<Site>& getSites() const;
  const SiteBase& chooseRandSite(int bondLength) const override;
  std::vector<const SiteBase*> getNearestNeighbors(
      const SiteBase& site) const override;
  int getTotNumSites() const override;
  const int getNumUniqueBonds(int bondLength = 2) const override;

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
  int chooseStartInd(consts::DirsType direc, int bondLength) const;

};