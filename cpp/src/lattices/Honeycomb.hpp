#pragma once

#include "LatticeBase.hpp"

#include <iostream>


class Honeycomb : public LatticeBase {
public:
  struct Site : public SiteBase {
    // xi and yi are integer coordinates of the Honeycomb primitive cell.
    // Indexing of xi and yi begins at 0. zi is 0 or 1, representing the two 
    // atoms in the primitive cell. 
    
    Site(int xi, int yi, int zi) : SiteBase(xi, yi, zi) {};
  };

  Honeycomb(const LatticeInput& input);

  int getSiteInd(int xi, int yi, int zi) const override;
  const Site& getSite(int xi, int yi, int zi) const override;
  int getTotNumSites() const override;
  const std::vector<Site>& getSites() const;
  const SiteBase& chooseRandSite(int bondLength = 2) const override;
  std::vector<const SiteBase*> getNearestNeighbors(
      const SiteBase& site) const override;
  const int getNumUniqueBonds(int bondLength = 2) const override;

private:
  int xNCells, yNCells, sitesPerCell;
  std::vector<Site> sites;

  std::vector<Site> createHoneycomb(const LatticeInput& input);
  int chooseStartInd(consts::DirsType direc, int bondLength) const override;
  
};