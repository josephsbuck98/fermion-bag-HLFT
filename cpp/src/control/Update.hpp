#pragma once

#include "Constants.hpp"
#include "RandomNEW.hpp" // Even though Random & TVModel aren't directly used 
#include "TVModelNEW.hpp" // here, files including Update.hpp need them. Perhaps 
                      // import them in Sweep.hpp?

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