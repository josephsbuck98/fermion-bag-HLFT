#include "Honeycomb.hpp"

#include <iostream>


using HC = Honeycomb;
using Site = HC::Site;


Honeycomb::Honeycomb(const LatticeInput& input)
    : LatticeBase(input) {
  xNCells = input.xNSites; yNCells = input.yNSites; sitesPerCell = 2;
  sites = createHoneycomb(input);
}

std::vector<Site> Honeycomb::createHoneycomb(const LatticeInput& input) {
  //TODO: This is identical to createSimpleCubic, apart from the 
  //TODO: newSite.size()) check. Could move the function to LatticeBase, 
  //TODO: although we might have more complicated lattices in the future. 
  //TODO: Probably a good idea to have this function callable in LatticeBase, 
  //TODO: and just call it for lattices that need it.
  std::vector<Site> newSites; 
  for (int zi = 0; zi < sitesPerCell; zi++) {
    for (int yi = 0; yi < yNCells; yi++) {
      for (int xi = 0; xi < xNCells; xi++) {
        newSites.push_back(Site(xi, yi, zi));
      }
    }
  }
  if (newSites.size() != xNCells * yNCells * sitesPerCell) {
    throw std::runtime_error("HoneyComb: The number of sites was not equal "
      "to the product of cell number and sites per cell.");
  }
  return newSites;
}

int Honeycomb::getTotNumSites() const {
  return sites.size();
}

int Honeycomb::getSiteInd(int xi, int yi, int zi) const {
  return 0;
}