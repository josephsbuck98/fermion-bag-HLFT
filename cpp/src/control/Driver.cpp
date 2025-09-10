#include <cmath>
#include <iostream>

#include "Constants.hpp"
#include "Driver.hpp"


double calcError(int origVal, int newVal);

//TODO: INPUT CURRENT SWEEP ITERATION IN CASE THIS IS A RESTART
Driver::Driver(InputParser::ParsedInput input) {
  ControlInput controlInput = input.controlInput;

  this->input = input;
  equilSweepsPatience = controlInput.equilSweepsPatience; 
  equilSweepsTol = controlInput.equilSweepsTol; 
  scaleNumUpdates = controlInput.scaleNumUpdates;
  maxSweeps = controlInput.maxSweeps;
  initNumTimeGroups = controlInput.initNumTimeGroups;
}

void Driver::run(Configuration& configuration, const Lattice& lattice) {
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

  int outSweepsPatience = input.outputInput.outSweepsPatience;
  std::vector<Sweep> sweeps;
  for (int sweep_iter = 0; sweep_iter < maxSweeps; sweep_iter++) {
    // Equilibration actions
    //TODO: LOG EQUILIBRATION DATA
    if (equilibrationSweeps < 0) { // If equilibration hasn't been reached...
      updateEquilTestingParams(configuration.getNumBonds());
    }
    if (currNumSweepsWithinTol >= equilSweepsPatience) { // If patience exceeded
      equilibrationSweeps = sweep_iter; // Don't add 1, b/c on new iter anyways
    }

    // Run current sweep
    Sweep newSweep = Sweep(input, sweep_iter);
    newSweep.run(configuration, lattice);

    // Handle data output logic
    sweeps.push_back(newSweep);
    if (sweeps.size() == outSweepsPatience - 1 || sweep_iter == maxSweeps - 1) {
      //TODO: Move this logic to Output
    }


    //TODO: Log equilibration data and any other data.
  }
}

double calcError(int origVal, int newVal) {
  return std::abs((origVal - newVal) / static_cast<double>(origVal));
}