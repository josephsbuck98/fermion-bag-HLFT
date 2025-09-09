#pragma once

#include "Constants.hpp"
#include "Random.hpp"
#include "TVModel.hpp"


template<typename HamiltonianType>
class Update {
public:
  Update(double lowerBound, double upperBound) {
    groupLowerBound = lowerBound;
    groupUpperBound = upperBound;
  }

  consts::BondActionType run(Configuration& configuration, const Lattice& lattice, 
      const HamiltonianType& hamiltonian) {
    return hamiltonian.applyUpdate(configuration, lattice, groupLowerBound, 
        groupUpperBound);
  }

private:
  double groupLowerBound;
  double groupUpperBound;

};