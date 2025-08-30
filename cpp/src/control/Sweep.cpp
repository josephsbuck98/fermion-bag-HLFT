#include <iostream>

#include "Sweep.hpp"


Sweep::Sweep(int initNumTimeGroups, double scaleNumUpdates) { //TODO: Just pass in controlInput?
  this->initNumTimeGroups = initNumTimeGroups;
  this->scaleNumUpdates = scaleNumUpdates;
}


void Sweep::run(Configuration& configuration, const Lattice& lattice) {
  // Calculate the number of time groups
  numTimeGroups = configuration.calcNumTimeGroups(initNumTimeGroups);

  // Create the new vector of tau starts, and save to configuration
  std::vector<double> newTauGroupStarts = generateTauGroupStarts( //TODO: CHANGE THIS TO A MEMBER FUNCTION
      configuration.getBeta(), numTimeGroups);
  configuration.setTauGroupStarts(newTauGroupStarts);

  // Calculate the number of updates per group per sweep to do
  int numUpdatesPerGroup = calcNumUpdatesPerGroup(scaleNumUpdates, 
      configuration.getBeta(), lattice.getNumSites(consts::DirsType::X), //TODO: In the future, getNumSites should automatically detect the lattice structure, and use its points to compute total sites
      numTimeGroups);
  //TODO: Compute numUpdatesPerGroup; beta from configuration, width from beta and initNumTimeGroups, Nsites from lattice, scaleNumUpdates from input. Update configuration's tauGroupStarts and numTimeGroups
  //TODO: Outer loop: loop over all taus in order. Store a start index. Once a tau exceeds the next group bound, store that as startNext. Create a set of taus from that slice of the vector. 
  //TODO: Inner loop: Once a newStart is found, loop numUpdatesPerGroup, and call update.run() on each. Store data as you go


  //TODO: Store Update object and relevant loop data.
}


int calcNumUpdatesPerGroup(double scale, double beta, 
    int numSites, int numTimeGroups) {
  double width = beta  / numTimeGroups;
  return scale * width * numSites;
}