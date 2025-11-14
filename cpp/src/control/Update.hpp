#pragma once

#include "Constants.hpp"
#include "Random.hpp"
#include "TVModel.hpp"


template<typename HamiltonianType>
class Update {
public:
  Update(int num, Configuration::RegionData& regionData) {
    this->regionData = regionData;
    groupNum = num;
  }

  consts::BondActionType run(Configuration& configuration, 
      const HamiltonianType& hamiltonian, const LatticeBase* lattice) {
    return hamiltonian.applyUpdate(configuration, groupNum, this->regionData, lattice);
  }

private:
  Configuration::RegionData regionData;
  int groupNum;

};