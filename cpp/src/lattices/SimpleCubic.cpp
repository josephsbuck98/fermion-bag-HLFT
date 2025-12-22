#include "SimpleCubic.hpp"
#include <iostream>


using SC = SimpleCubic;
using Site = SC::Site;


void SimpleCubic::printInfo() const {
  std::cout << "Simple Cubic Lattice (" << getNumSites(consts::DirsType::X) 
      << std::endl;
}

SimpleCubic::SimpleCubic(const LatticeInput& input) : LatticeBase(input) { 
  //TODO: Initialize mins, lat params
  xNSites = input.xNSites; yNSites = input.yNSites; zNSites = input.zNSites;
  dims = input.dims;
  sites = createSimpleCubic(input);
  sitesNEW = createSimpleCubicNEW(input);
};

std::unordered_map<consts::DirsType, std::vector<double>, 
    std::EnumClassHash<consts::DirsType>> 
    SimpleCubic::createSimpleCubic(LatticeInput input) {

  std::unordered_map<consts::DirsType, std::vector<double>, 
      std::EnumClassHash<consts::DirsType>> new_sites;
  
  // X, Y, and Z
  double min = input.xMin; double base = input.a; int nsites = input.xNSites;
  new_sites[consts::DirsType::X] = genUniform1DLattice(min, base, nsites);

  // Y and Z
  if (dims == consts::DimsType::TWO || dims == consts::DimsType::THREE) {
    min = input.yMin; base = input.b; nsites = input.yNSites;
    new_sites[consts::DirsType::Y] = genUniform1DLattice(min, base, nsites);
  }

  // Z
  if (dims == consts::DimsType::THREE) {
    min = input.zMin; base = input.c; nsites = input.zNSites;
    new_sites[consts::DirsType::Z] = genUniform1DLattice(min, base, nsites);
  }

  return new_sites;
}

std::vector<Site> SC::createSimpleCubicNEW(const LatticeInput& input) {
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

std::vector<double> SimpleCubic::genUniform1DLattice(double min, double base, int nsites) {
  std::vector<double> new_lattice(nsites);
  new_lattice[0] = min;
  for (int i = 1; i < nsites; i++) {
    new_lattice[i] = new_lattice[i - 1] + base;
  }
  return new_lattice;
}

int SimpleCubic::getNumSites(consts::DirsType dir) const {
  return sites.at(dir).size();
};

double SimpleCubic::getSite(consts::DirsType dir, int index) const {
  return sites.at(dir)[index];
};

const Site& SimpleCubic::getSiteNEW(int xi, int yi, int zi) const {
  static const Site invalidSite{-1, -1, -1};
  int index = xi + xNSites * (yi + yNSites * zi);
  if (index < sitesNEW.size()) {
    return sitesNEW[index];
  } else {
    std::string eMS = "SimpleCubic: Indices " + std::to_string(xi) + ", " 
        + std::to_string(yi) + ", " + std::to_string(zi) 
        + " do not exist in the lattice. ";
    // throw std::runtime_error(eMS);
    return invalidSite;
  }
}

const std::vector<Site>& SimpleCubic::getSitesNEW() const {
  return sitesNEW;
}

std::vector<Site> SimpleCubic::getNearestNeighborsNEW(const Site& site) {
  std::vector<Site> nearestNeighbors;

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
    Site neighborSite = getSiteNEW(neighborIndices[0], neighborIndices[1], 
        neighborIndices[2]);
    nearestNeighbors.push_back(neighborSite);
  }
  
  return nearestNeighbors;
}

int SimpleCubic::getTotNumSitesNEW() const {
  return xNSites * yNSites * zNSites;
}

consts::BoundType SimpleCubic::getBoundType(consts::DirsType dir) const {
  return boundTypes.at(dir);
}