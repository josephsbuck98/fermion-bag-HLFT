#pragma once

#include <vector>

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
  // consts::HamilModel hamilModel;
  int stopSweepsPatience;
  double stopSweepsTol;
  double scaleNumUpdates;
  int maxSweeps;
  int initNumTimeGroups; //TODO: Might not need this here?

  // double acceptProb = 0.5;
  // double insertProb = 0.5;       

  // Data members for dynamically controlling loop execution
  int currNumSweepsWithinTol = 0;
  int currStableNbonds = 0;

  // Data members for storing loop iteration data (sweep objects, etc)
  std::vector<Sweep> sweeps;
};