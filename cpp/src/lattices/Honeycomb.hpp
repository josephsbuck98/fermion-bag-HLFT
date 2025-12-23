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
    //DELETE ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // int xi, yi, zi;
    // Site(int xii, int yii, int zii): xi(xii), yi(yii), zi(zii) {}
  };

  explicit Honeycomb(const LatticeInput& input);

  int getNumSites(consts::DirsType dir) const override;

  //TODO: IMPLEMENT
  const Site& getSite(int xi, int yi, int zi) const {
    return sites[0];
  }

  consts::BoundType getBoundType(consts::DirsType dir) const override;

  void printInfo() const override;

private:
  int nCells; //TODO: Instead, specify nCells in each dimension (x and y)
  std::vector<Site> sites = {Site(-1, -1, -1)};
  
};