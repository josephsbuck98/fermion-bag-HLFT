#include "Honeycomb.hpp"
#include "RandomHelpers.hpp"

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

int Honeycomb::getSiteInd(int xi, int yi, int zi) const {
  if (xi < 0 || xi >= xNCells || yi < 0 || yi >= yNCells 
      || zi < 0 || zi >= sitesPerCell) {
    std::string eMS = "Honeycomb: Indices " + std::to_string(xi) + ", " 
        + std::to_string(yi) + ", " + std::to_string(zi) 
        + " contain an integer out of the range. ";
    throw std::runtime_error(eMS);
  }
  int index = xi + xNCells * (yi + yNCells * zi);
  if (index < sites.size()) {
    return index;
  } else {
    std::string eMS = "Honeycomb: Indices " + std::to_string(xi) + ", " 
        + std::to_string(yi) + ", " + std::to_string(zi) 
        + " do not exist in the lattice. ";
    throw std::runtime_error(eMS);
  }
}

const Site& Honeycomb::getSite(int xi, int yi, int zi) const {
  static const Site invalidSite{-1, -1, -1};
  try {
    int siteInd = getSiteInd(xi, yi, zi);
    return sites[siteInd];
  } catch (const std::runtime_error& err) {
    return invalidSite; 
  }
}

const std::vector<Site>& Honeycomb::getSites() const {
  return sites;
}

const SiteBase& Honeycomb::chooseRandSite(int bondLength) const {
  int xStartInd = chooseStartInd(consts::DirsType::X, bondLength); 
  int yStartInd = chooseStartInd(consts::DirsType::Y, bondLength); 
  int zStartInd = chooseStartInd(consts::DirsType::Z, bondLength);
  return getSite(xStartInd, yStartInd, zStartInd); 
}

std::vector<const SiteBase*> Honeycomb::getNearestNeighbors(const SiteBase& site) const {
  std::vector<const SiteBase*> nearestNeighbors;

  int shift = 2 * site.zi - 1; // 1->1, 0->-1
  int neighborZi = !site.zi; // 1->0, 0->1

  int neighborXi = site.xi + shift;
  if (input.xBCType == consts::BoundType::PERIODIC) {
    neighborXi = (neighborXi + xNCells) % xNCells;
  }

  int neighborYi = site.yi + shift;
  if (input.yBCType == consts::BoundType::PERIODIC) {
    neighborYi = (neighborYi + yNCells) % yNCells;
  }

  std::vector<const SiteBase*> candidates;
  candidates.push_back(&getSite(neighborXi, site.yi, neighborZi));
  candidates.push_back(&getSite(site.xi, neighborYi, neighborZi));
  candidates.push_back(&getSite(neighborXi, neighborYi, neighborZi));

  for (int i = 0; i < candidates.size(); i++) {
    if (candidates[i] && candidates[i]->xi >= 0) {
      nearestNeighbors.push_back(candidates[i]);
    }
  }

  return nearestNeighbors;
}

const int Honeycomb::getNumUniqueBonds(int bondLength) const {
  int nBonds = 3 * getTotNumSites() / 2;
  if (input.xBCType == consts::BoundType::OPEN) {
    nBonds -= 2 * yNCells;
  }
  if (input.yBCType == consts::BoundType::OPEN) {
    nBonds -= 2 * xNCells;
  }
  return nBonds;
}

int Honeycomb::getTotNumSites() const {
  return sites.size();
}

int Honeycomb::chooseStartInd(consts::DirsType direc, int bondLength) const {
  int numOpts;
  if (direc == consts::DirsType::X) {numOpts = xNCells;}
  else if (direc == consts::DirsType::Y) {numOpts = yNCells;}
  else {numOpts = sitesPerCell;}

  if (direc == consts::DirsType::Z) {
    return chooseUnifRandIntWithBounds(0, numOpts);
  }
  
  consts::BoundType bcType = getBoundType(direc);
  if (bcType == consts::BoundType::OPEN) {
    return chooseUnifRandIntWithBounds(0, numOpts - bondLength + 1);
  } else {
    return chooseUnifRandIntWithBounds(0, numOpts);
  }
}