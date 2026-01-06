#pragma once

#include "LatticeBase.hpp"

#include <iostream>


class SimpleCubic : public LatticeBase {
public:
  struct Site : public SiteBase {
    // xi, yi, and zi are integer coordinates of the atoms in the SimpleCubic 
    // lattice. 

    Site(int xi, int yi, int zi) : SiteBase(xi, yi, zi) {};
  };

  SimpleCubic(const LatticeInput& input);

  int getSiteInd(int xi, int yi, int zi) const override;
  const Site& getSite(int xi, int yi, int zi) const override;
  const std::vector<Site>& getSites() const;
  const SiteBase& chooseRandSite(int bondLength) const override;
  std::vector<const SiteBase*> getNearestNeighbors(
      const SiteBase& site) const override;
  int getTotNumSites() const override;
  const int getNumUniqueBonds(int bondLength = 2) const override;

  
private:
  consts::DimsType dims;

  std::vector<Site> sites; 
  int xNSites, yNSites, zNSites;
  double xMin, yMin, zMin;
  double a, b, c;
  double alpha, beta, gamma;

  std::vector<Site> createSimpleCubic(const LatticeInput& input);
  int chooseStartInd(consts::DirsType direc, int bondLength) const;

};