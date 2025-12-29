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

//DELETE ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// std::unordered_map<consts::DirsType, std::ector<double>, 
//     std::EnumClassHash<consts::DirsType>> 
//     SimpleCubic::createSimpleCubic(LatticeInput input) {

//   std::unordered_map<consts::DirsType, std::vector<double>, 
//       std::EnumClassHash<consts::DirsType>> new_sites;
  
//   // X, Y, and Z
//   double min = input.xMin; double base = input.a; int nsites = input.xNSites;
//   new_sites[consts::DirsType::X] = genUniform1DLattice(min, base, nsites);

//   // Y and Z
//   if (dims == consts::DimsType::TWO || dims == consts::DimsType::THREE) {
//     min = input.yMin; base = input.b; nsites = input.yNSites;
//     new_sites[consts::DirsType::Y] = genUniform1DLattice(min, base, nsites);
//   }

//   // Z
//   if (dims == consts::DimsType::THREE) {
//     min = input.zMin; base = input.c; nsites = input.zNSites;
//     new_sites[consts::DirsType::Z] = genUniform1DLattice(min, base, nsites);
//   }

//   return new_sites;
// }

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


//DELETE ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// std::vector<double> SimpleCubic::genUniform1DLattice(double min, double base, int nsites) {
//   std::vector<double> new_lattice(nsites);
//   new_lattice[0] = min;
//   for (int i = 1; i < nsites; i++) {
//     new_lattice[i] = new_lattice[i - 1] + base;
//   }
//   return new_lattice;
// }

int SimpleCubic::getNumSites(consts::DirsType dir) const {
  if (dir == consts::DirsType::X) return input.xNSites;
  else if (dir == consts::DirsType::Y) return input.yNSites;
  else {return input.zNSites;};
};



//DELETE ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// double SimpleCubic::getSite(consts::DirsType dir, int index) const {
//   return sites.at(dir)[index];
// };



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
  int startInd = 0;
  int nSites = 1;
  if (direc == consts::DirsType::X) {nSites = xNSites;}
  else if (direc == consts::DirsType::Y) {nSites = yNSites;}
  else {nSites = zNSites;}
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

const int SimpleCubic::getNumUniqueBonds() const {
  int bondLength = 2; //TODO: Make a default parameter
  int nBonds = getTotNumSites();
  //TODO: Don't need *3 or *2 if bond length is 1.
  if (yNSites >= bondLength && zNSites >= bondLength) {
    nBonds *= 3; // Three bonds per site in 3D SC.
  } else if (yNSites >= bondLength || zNSites > bondLength) {
    nBonds *= 2; // Two bonds per site in 2D SC. Assumes x is ALWAYS a dimension
  }
  if (input.xBCType == consts::BoundType::OPEN) {
    nBonds -= ((bondLength - 1) * yNSites * zNSites);
  } else if (input.yBCType == consts::BoundType::OPEN) {
    nBonds -= ((bondLength - 1) * xNSites * zNSites);
  } else if (input.zBCType == consts::BoundType::OPEN) {
    nBonds -= ((bondLength - 1) * xNSites * yNSites);
  }
  return nBonds;
}

consts::BoundType SimpleCubic::getBoundType(consts::DirsType dir) const {
  return boundTypes.at(dir);
}