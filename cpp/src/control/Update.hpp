#pragma once

#include "Constants.hpp"
#include "Random.hpp"
#include "TVModel.hpp"


template<typename HamiltonianType>
class Update {
public:
  Update(double lowerBound, double upperBound, int num) {
    groupLowerBound = lowerBound;
    groupUpperBound = upperBound;
    groupNum = num;
  }

  consts::BondActionType run(Configuration& configuration, const Lattice& lattice, 
      const HamiltonianType& hamiltonian) {
    return hamiltonian.applyUpdate(configuration, lattice, groupLowerBound, 
        groupUpperBound, groupNum);
  }

private:
  double groupLowerBound;
  double groupUpperBound;
  int groupNum;

};