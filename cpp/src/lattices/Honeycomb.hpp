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
  int getTotNumSites() const override;

  const int getNumUniqueBonds(int bondLength) const override {return 0;};

  const Site& getSite(int xi, int yi, int zi) const override {
    return sites[0];
  }


  const SiteBase& chooseRandSite(int bondLength = 2) const override {
    return sites[0];
  };
  std::vector<const SiteBase*> getNearestNeighbors(const SiteBase& site) const override {
    std::vector<const SiteBase*> neighbors;
    for (auto& site : sites) {
      neighbors.push_back(&site);
    }
    return neighbors;
  };

private:
  int xNCells, yNCells, sitesPerCell;
  std::vector<Site> sites;

  std::vector<Site> createHoneycomb(const LatticeInput& input);
  
};