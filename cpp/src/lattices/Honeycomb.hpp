#pragma once

#include "LatticeBase.hpp"

#include <iostream>


class Honeycomb : public LatticeBase {
public:
  struct Site {
    // xi and yi are integer coordinates of the Honeycomb primitive cell.
    // Indexing of xi and yi begins at 0. bi is 0 or 1, representing the two 
    // atoms in the primitive cell. 
    int xi, yi, bi;
    Site(int xii, int yii, int bii): xi(xii), yi(yii), bi(bii) {}
  };

  explicit Honeycomb(const LatticeInput& input);

  int getNumSites(consts::DirsType dir) const override;

  consts::BoundType getBoundType(consts::DirsType dir) const override;

  void printInfo() const override;

private:
  int nCells; //TODO: Instead, specify nCells in each dimension (x and y)

  std::vector<Site> sitesNEW;
  
};