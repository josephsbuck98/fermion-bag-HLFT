#include <iostream>

#include "Sweep.hpp"


Sweep::Sweep(int initNumTimeGroups, double scaleNumUpdates) { //TODO: Just pass in controlInput?
  initNumTimeGroups = initNumTimeGroups;
  scaleNumUpdates = scaleNumUpdates;
}


void Sweep::run(Configuration& configuration, const Lattice& lattice) {
  //TODO: Compute numUpdatesPerGroup; beta from configuration, width from beta and initNumTimeGroups, Nsites from lattice, scaleNumUpdates from input
  //TODO: Outer loop: loop over all taus in order. Store a start index. Once a tau exceeds the next group bound, store that as startNext. Create a set of taus from that slice of the vector. 
  //TODO: Inner loop: Once a newStart is found, loop numUpdatesPerGroup, and call update.run() on each. Store data as you go
  //TODO: 


  //TODO: Loop over the time groups (tauGroupStarts in Configuration has the bounds of each grouping)
  //TODO: Within each loop, calculate the number of updates to propose (scaleNumUpdates * Nsites * groupwidth)
  //TODO: Loop over the number of updates
  //TODO: Create an Update object and call its .run() function.
  //TODO: Store Update object and relevant loop data.
}