#include <iostream>

#include "Constants.hpp"
#include "Driver.hpp"


Driver::Driver(ControlInput input) {
  hamilModel = input.hamil_model;
  nbondsStopSweeps = input.nbonds_stop_sweeps; //TODO: Rename nbondsStopSweeps to stopSweepsPatience
  nbondsStopTol = input.nbonds_stop_tol; //TODO: Rename nbondsStopTol to stopSweepsTol
  scaleUpdatesPerSweep = input.scale_updates_per_sweep;
  maxSweeps = input.max_sweeps;
  insertProb = input.insert_prob; //TODO: Probably belongs in Hamiltonians somewhere, but might be ok here and passing to Hamiltonian constr later.
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