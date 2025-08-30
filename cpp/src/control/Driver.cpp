#include <iostream>

#include "Constants.hpp"
#include "Driver.hpp"


Driver::Driver(ControlInput input) {
  // hamilModel = input.hamil_model;
  stopSweepsPatience = input.stopSweepsPatience; 
  stopSweepsTol = input.stopSweepsTol; 
  scaleNumUpdates = input.scaleNumUpdates;
  maxSweeps = input.max_sweeps;
  initNumTimeGroups = input.initNumTimeGroups;

  // acceptProb = input.acceptProb;
  // insertProb = input.insertProb;
}

void Driver::run(Configuration curr_configuration, Lattice lattice) {
  for (int sweep_iter = 0; sweep_iter < maxSweeps; sweep_iter++) {
    //TODO: Check if new nbonds is within nbondsStopTol
    //TODO: Check if currStableNbonds has been exceeded. 
    //TODO: Update currNumSweepsWithinTol and currStableNbonds for all cases
    //TODO: Create a Sweep object and call its .run() function
    //TODO: Store sweep object and other loop iteration data
  }
}