#pragma once

#include "Constants.hpp"
#include "Input.hpp"


struct SiteBase {
  // xi, yi, and zi are integer coordinates of the atoms in the SimpleCubic 
  // lattice. In the Honeycomb lattice, xi and yi are the coordinates of a 
  // cell, and zi is the 0 or 1 for the first or second site of the basis.
  int xi, yi, zi;
  SiteBase(int xii, int yii, int zii): xi(xii), yi(yii), zi(zii) {}
  friend std::ostream& operator<<(std::ostream& os, const SiteBase& site) {
    os << std::to_string(site.xi) << ", " << std::to_string(site.yi) 
        << ", " << std::to_string(site.zi);
    return os;
  }
};

class LatticeBase {
public:
  LatticeBase(const LatticeInput& input) : input(input), type(input.type) {
    boundTypes[consts::DirsType::X] = input.xBCType;
    boundTypes[consts::DirsType::Y] = input.yBCType;
    boundTypes[consts::DirsType::Z] = input.zBCType;
  };

  virtual ~LatticeBase() = default;
  virtual void printInfo() const = 0;
  virtual int getNumSites(consts::DirsType dir) const = 0;
  virtual int getTotNumSites() const = 0;
  virtual const int getNumUniqueBonds(int bondLength = 2) const = 0;

  virtual int getSiteInd(int x, int y, int z) const = 0;
  virtual const SiteBase& getSite(int x, int y, int z) const = 0;
  virtual const SiteBase& chooseRandSite(int bondLength = 2) const = 0;
  virtual std::vector<const SiteBase*> getNearestNeighbors(const SiteBase& site) const = 0;

  virtual consts::BoundType getBoundType(consts::DirsType dir) const = 0;

protected:
  LatticeInput input;

  consts::LatticeType type;

  std::unordered_map<consts::DirsType, consts::BoundType, 
    std::EnumClassHash<consts::DirsType>> boundTypes;

};