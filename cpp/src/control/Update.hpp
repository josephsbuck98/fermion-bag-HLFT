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

  consts::BondActionType run(Configuration& configuration, const Lattice& lattice, 
      const HamiltonianType& hamiltonian) {
    return hamiltonian.applyUpdate(configuration, lattice, groupNum, this->regionData);
  }

private:
  Configuration::RegionData regionData;
  int groupNum;

};