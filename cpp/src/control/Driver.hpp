#pragma once

#include "Configuration.hpp"
#include "Input.hpp"
#include "Lattice.hpp"
#include "Sweep.hpp"


class Driver {
public:
  Driver(ControlInput input);

  void run(Configuration curr_configuration, Lattice lattice);

private:
  // Data members read directly from input
  consts::HamilModel hamilModel;
  int nbondsStopSweeps;
  double nbondsStopTol;
  double scaleUpdatesPerSweep;
  int maxSweeps;
  double insertProb = 0.5;          // For 'random' hamiltonian model

  // Data members for dynamically controlling loop execution
  int currNumSweepsWithinTol = 0;
  int currStableNbonds = 0;

  // Data members for storing loop iteration data (sweep objects, etc)
};