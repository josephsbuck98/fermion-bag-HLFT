#pragma once

#include "Constants.hpp"
#include "Random.hpp"
#include "TVModel.hpp"


template<typename HamiltonianType>
class Update {
public:
  Update(double lowerBound, double upperBound, int num, Configuration::RegionData& regionData) {
    this->regionData = regionData;
    groupLowerBound = lowerBound; //TODO: Remove these
    groupUpperBound = upperBound; //TODO: Remove these
    groupNum = num;
    //TODO: Update to use only regionData, or perhaps 
  }

  consts::BondActionType run(Configuration& configuration, const Lattice& lattice, 
      const HamiltonianType& hamiltonian) {
    return hamiltonian.applyUpdate(configuration, lattice, groupLowerBound, 
        groupUpperBound, groupNum, this->regionData);
  }

private:
  Configuration::RegionData regionData;
  double groupLowerBound;
  double groupUpperBound;
  int groupNum;

};