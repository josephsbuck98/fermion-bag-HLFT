#include <iostream>

#include "Constants.hpp"
#include "Driver.hpp"


Driver::Driver(ControlInput input) {
  hamilModel = input.hamil_model;
  stopSweepsPatience = input.stopSweepsPatience; 
  stopSweepsTol = input.stopSweepsTol; 
  scaleNumUpdates = input.scaleNumUpdates;
  maxSweeps = input.max_sweeps;
  initNumTimeGroups = input.initNumTimeGroups;

  acceptProb = input.acceptProb; //TODO: Probably belongs in Hamiltonians somewhere, but might be ok here and passing to Hamiltonian constr later.
  insertProb = input.insertProb; //TODO: Probably belongs in Hamiltonians somewhere, but might be ok here and passing to Hamiltonian constr later.
}

void Driver::run(Configuration curr_configuration, Lattice lattice) {
  for (int sweep_iter = 0; sweep_iter < maxSweeps; sweep_iter++) {
    //TODO: Implement checking if new nbonds is within nbondsStopTol
    //TODO: Implement checking if currStableNbonds has been exceeded
    //TODO: Implement updating currNumSweepsWithinTol and currStableNbonds for all cases
    //TODO: Implement storage of sweep object and other loop iteration data
    //TODO: Implement 
  }
}