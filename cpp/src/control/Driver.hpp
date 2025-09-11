#pragma once

#include <vector>

#include "Output.hpp"
#include "Sweep.hpp"


class Driver {
public:
  Driver(InputParser::ParsedInput input);

  void run(Configuration& curr_configuration, const Lattice& lattice,
      Output& output);

private:
  InputParser::ParsedInput input;

  // "Static" data members read directly from input
  int equilSweepsPatience;
  double equilSweepsTol;
  double scaleNumUpdates;
  int maxSweeps;
  int initNumTimeGroups; //TODO: Might not need this here?

  // Data members for computing equilibration time
  int currNumSweepsWithinTol = 0;
  int currStableNbonds = 0;

  // Equilibration time
  double equilibrationTime = -1.0;
  int equilibrationSweeps = -1;

  // Data members for storing loop iteration data (sweep objects, etc)
  std::vector<Sweep> sweeps;
};