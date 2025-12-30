#include "RandomHelpers.hpp"
#include "SimpleCubic.hpp"

#include <iostream>


using SC = SimpleCubic;
using Site = SC::Site;


void SimpleCubic::printInfo() const {
  std::cout << "Simple Cubic Lattice (" <<  getTotNumSites()
      << std::endl;
}

SimpleCubic::SimpleCubic(const LatticeInput& input) : LatticeBase(input) { 
  //TODO: Initialize mins, lat params
  xNSites = input.xNSites; yNSites = input.yNSites; zNSites = input.zNSites;
  dims = input.dims;
  // sites = createSimpleCubic(input); //DELETE ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  sites = createSimpleCubic(input);
};

std::vector<Site> SimpleCubic::createSimpleCubic(const LatticeInput& input) {
  std::vector<Site> new_sites; 
  for (int zi = 0; zi < zNSites; zi++) {
    for (int yi = 0; yi < yNSites; yi++) {
      for (int xi = 0; xi < xNSites; xi++) {
        new_sites.push_back(Site(xi, yi, zi));
      }
    }
  }
  if (new_sites.size() != xNSites * yNSites * zNSites) {
    throw std::runtime_error("SimpleCubic: The number of sites was not equal "
      "to the product of sites in each dimension.");
  }
  return new_sites;
}

int SimpleCubic::getNumSites(consts::DirsType dir) const {
  if (dir == consts::DirsType::X) return input.xNSites;
  else if (dir == consts::DirsType::Y) return input.yNSites;
  else {return input.zNSites;};
}

int SimpleCubic::getSiteInd(int xi, int yi, int zi) const {
  int index = xi + xNSites * (yi + yNSites * zi);
  if (index < sites.size()) {
    return index;
  } else {
    std::string eMS = "SimpleCubic: Indices " + std::to_string(xi) + ", " 
        + std::to_string(yi) + ", " + std::to_string(zi) 
        + " do not exist in the lattice. ";
    throw std::runtime_error(eMS);
  }
}

const Site& SimpleCubic::getSite(int xi, int yi, int zi) const {
  static const Site invalidSite{-1, -1, -1};
  try {
    int siteInd = getSiteInd(xi, yi, zi);
    return sites[siteInd];
  } catch (const std::runtime_error& err) {
    return invalidSite;
  }
}

const std::vector<Site>& SimpleCubic::getSites() const {
  return sites;
}

int SimpleCubic::chooseStartInd(consts::DirsType direc, int bondLength) const {
  int nSites;
  if (direc == consts::DirsType::X) {nSites = xNSites;}
  else if (direc == consts::DirsType::Y) {nSites = yNSites;}
  else {nSites = zNSites;}

  // This is for the case where the dimension whose indices we are determining 
  // is not part of the simulation (for example, choosing a z index when 
  // working in 1D or 2D)
  if (nSites == 1) {return 0;}
  
  consts::BoundType bcType = getBoundType(direc);
  if (bcType == consts::BoundType::OPEN) {
    return chooseUnifRandIntWithBounds(0, nSites - bondLength + 1);
  } else {
    return chooseUnifRandIntWithBounds(0, nSites);
  }
}

const SiteBase& SimpleCubic::chooseRandSite(int bondLength) const {
  int xStartInd = chooseStartInd(consts::DirsType::X, bondLength); 
  int yStartInd = chooseStartInd(consts::DirsType::Y, bondLength); 
  int zStartInd = chooseStartInd(consts::DirsType::Z, bondLength);
  return getSite(xStartInd, yStartInd, zStartInd); 
}

std::vector<const SiteBase*> SimpleCubic::getNearestNeighbors(const SiteBase& site) const {
  std::vector<const SiteBase*> nearestNeighbors;

  struct Helper {
    int index; int nSites; consts::BoundType bc;
    Helper(int index, int nSites, consts::BoundType bc) : index(index), 
        nSites(nSites), bc(bc) {}
  };
  std::vector<Helper> helpers;
  helpers.push_back(Helper(site.xi, xNSites, input.xBCType));
  helpers.push_back(Helper(site.yi, yNSites, input.yBCType));
  helpers.push_back(Helper(site.zi, zNSites, input.zBCType));

  std::vector<int> indices = {site.xi, site.yi, site.zi};

  int nDims = keyFromValue<int, consts::DimsType>(consts::DIMS_TYPE_MAP, dims);
  for (int i = 0; i < nDims; i++) {
    std::vector<int> neighborIndices = indices;
    neighborIndices[i] += 1;
    if (neighborIndices[i] == helpers[i].nSites) {
      if (helpers[i].bc == consts::BoundType::PERIODIC) {
        neighborIndices[i] = 0;
      } else {
        continue;
      }
    }
    const Site& neighborSite = getSite(neighborIndices[0], neighborIndices[1], 
        neighborIndices[2]);
    nearestNeighbors.push_back(&neighborSite);
  }
  
  return nearestNeighbors;
}

int SimpleCubic::getTotNumSites() const {
  return xNSites * yNSites * zNSites;
}

const int SimpleCubic::getNumUniqueBonds(int bondLength) const {
  //TODO: Could refactor into a more intuitive structure.
  int nBonds = getTotNumSites();
  if (bondLength == 1) {return nBonds;}
  if (dims == consts::DimsType::TWO) {
    nBonds *= 2; // Two bonds per site in 2D SC.
  }
  if (dims == consts::DimsType::THREE) {
    nBonds *= 3; // Three bonds per site in 3D SC.
  }

  if (input.xBCType == consts::BoundType::OPEN) {
    nBonds -= ((bondLength - 1) * yNSites * zNSites);
  } 

  if (dims != consts::DimsType::ONE) {
    if (input.yBCType == consts::BoundType::OPEN) {
      nBonds -= ((bondLength - 1) * xNSites * zNSites);
    }
    if (dims == consts::DimsType::THREE) {
      if (input.zBCType == consts::BoundType::OPEN) {
        nBonds -= ((bondLength - 1) * xNSites * yNSites);
      }
    }
  }
  return nBonds;
}

consts::BoundType SimpleCubic::getBoundType(consts::DirsType dir) const {
  return boundTypes.at(dir);
}