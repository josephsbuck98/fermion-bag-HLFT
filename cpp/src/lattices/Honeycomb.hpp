#pragma once

#include "LatticeBase.hpp"

#include <iostream>


class Honeycomb : public LatticeBase {
public:
  struct Site : public SiteBase {
    // xi and yi are integer coordinates of the Honeycomb primitive cell.
    // Indexing of xi and yi begins at 0. bi is 0 or 1, representing the two 
    // atoms in the primitive cell. 
    
    Site(int xi, int yi, int zi) : SiteBase(xi, yi, zi) {};
  };

  explicit Honeycomb(const LatticeInput& input);

  int getNumSites(consts::DirsType dir) const override;
  const int getNumUniqueBonds(int bondLength) const override {return 0;};

  int getSiteInd(int x, int y, int z) const override {return 0;};
  const Site& getSite(int xi, int yi, int zi) const override {
    return sites[0];
  }

  consts::BoundType getBoundType(consts::DirsType dir) const override;

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
  int getTotNumSites() const {return 0;};

  void printInfo() const override;

private:
  int nCells; //TODO: Instead, specify nCells in each dimension (x and y)
  std::vector<Site> sites = {Site(-1, -1, -1)};
  
};