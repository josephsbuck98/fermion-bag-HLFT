#include <iostream>

#include "Sweep.hpp"


Sweep::Sweep(int initNumTimeGroups, double scaleNumUpdates) { //TODO: Just pass in controlInput?
  initNumTimeGroups = initNumTimeGroups;
  scaleNumUpdates = scaleNumUpdates;
}


void Sweep::run() {
  //TODO: Loop over the time groups (tauGroupStarts in Configuration has the bounds of each grouping)
  //TODO: Within each loop, calculate the number of updates to propose (scaleNumUpdates * Nsites * groupwidth)
  //TODO: Loop over the number of updates
  //TODO: Create an Update object and call its .run() function.
  //TODO: Store Update object and relevant loop data.
}