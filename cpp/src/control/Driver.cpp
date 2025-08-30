#include <cmath>
#include <iostream>

#include "Constants.hpp"
#include "Driver.hpp"


double calcError(int origVal, int newVal);

Driver::Driver(ControlInput input) {
  equilSweepsPatience = input.equilSweepsPatience; 
  equilSweepsTol = input.equilSweepsTol; 
  scaleNumUpdates = input.scaleNumUpdates;
  maxSweeps = input.max_sweeps;
  initNumTimeGroups = input.initNumTimeGroups;
}

void Driver::run(Configuration& configuration, const Lattice& lattice) { //TODO: Remove lattice as an input?
  auto calcError = [this](int currNbonds) {
    return std::abs((currStableNbonds - currNbonds) 
        / static_cast<double>(currStableNbonds));
  };

  auto updateEquilTestingParams = [this, &calcError](int currNbonds) {
    if (currStableNbonds != 0) {
      double stdErr = calcError(currNbonds);
      if (stdErr > equilSweepsTol) { // Start over window if % change exceeds tol
        currNumSweepsWithinTol = 0;
        currStableNbonds = currNbonds;
      } else { // Otherwise increment window count
        currNumSweepsWithinTol += 1;
      }
    }
  };

  std::vector<Sweep> sweeps;
  for (int sweep_iter = 0; sweep_iter < maxSweeps; sweep_iter++) {
    if (equilibrationSweeps < 0) { // If equilibration hasn't been reached...
      updateEquilTestingParams(configuration.getNumBonds());
    }
    if (currNumSweepsWithinTol >= equilSweepsPatience) { // If patience exceeded
      equilibrationSweeps = sweep_iter; // Don't add 1, b/c on new iter anyways
    }
    // Sweep newSweep = Sweep(initNumTimeGroups, scaleNumUpdates);
    // newSweep.run(configuration, lattice);
    // sweeps.push_back(newSweep);
    //TODO: Create a Sweep object and call its .run() function
    //TODO: Store sweep object and other loop iteration data
    //TODO: Log equilibration data and any other data.
  }
}

double calcError(int origVal, int newVal) {
  return std::abs((origVal - newVal) / static_cast<double>(origVal));
}