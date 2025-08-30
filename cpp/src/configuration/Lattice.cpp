#include <array>
#include <stdexcept>
#include <string>
#include <unordered_set>

#include "EnumHelpers.hpp"
#include "Lattice.hpp"

std::vector<double> genUniform1DLattice(double min, double base, int nsites);

Lattice::Lattice(LatticeInput input) {
  type = input.type;
  dims = input.dims;
  if (type == consts::LatticeType::SIMPLE_CUBIC) {
    sites = createSimpleCubic(input);
  } else {
    // sites = createHoneycomb(input);
  }
};

std::unordered_map<consts::DirsType, std::vector<double>, 
    std::EnumClassHash<consts::DirsType>> 
    Lattice::createSimpleCubic(LatticeInput input) {

  std::unordered_map<consts::DirsType, std::vector<double>, 
      std::EnumClassHash<consts::DirsType>> new_sites;
  
  // X, Y, and Z
  double min = input.x_min; double base = input.a; int nsites = input.x_nsites;
  new_sites[consts::DirsType::X] = genUniform1DLattice(min, base, nsites);

  // Y and Z
  if (dims == consts::DimsType::TWO || dims == consts::DimsType::THREE) {
    min = input.y_min; base = input.b; nsites = input.y_nsites;
    new_sites[consts::DirsType::Y] = genUniform1DLattice(min, base, nsites);
  }

  // Z
  if (dims == consts::DimsType::THREE) {
    min = input.z_min; base = input.c; nsites = input.z_nsites;
    new_sites[consts::DirsType::Z] = genUniform1DLattice(min, base, nsites);
  }

  return new_sites;
}

std::vector<double> genUniform1DLattice(double min, double base, int nsites) {
  std::vector<double> new_lattice(nsites);
  new_lattice[0] = min;
  for (int i = 1; i < nsites; i++) {
    new_lattice[i] = new_lattice[i - 1] + base;
  }
  return new_lattice;
}

int Lattice::getNumSites(consts::DirsType dir) const {
  return sites[dir].size();
};

double Lattice::getSite(consts::DirsType dir, int index) const {
  return sites.at(dir)[index];
};
