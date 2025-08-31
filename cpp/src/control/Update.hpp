#pragma once

#include "Constants.hpp"
#include "Random.hpp"


template<typename HamiltonianType>
class Update {
public:
  Update(double lowerBound, double upperBound) {
    groupLowerBound = lowerBound;
    groupUpperBound = upperBound;
  }

  void run(Configuration& configuration, Lattice& lattice, 
      const HamiltonianType& hamiltonian) {
    hamiltonian.applyUpdate(configuration, lattice);
  }

private:
  double groupLowerBound;
  double groupUpperBound;

};