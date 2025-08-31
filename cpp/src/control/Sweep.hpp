#pragma once

#include <map>
#include <vector>

#include "Configuration.hpp"
#include "InputParser.hpp"
#include "Lattice.hpp"
#include "Random.hpp"
#include "TVModel.hpp"

class Sweep { //This is the only class that will both define the loop structure AND handle the majority of the inner workings of each iteration. It will loop over the different time groupings, and within each grouping it will loop for a specified number of times, calling update each iteration.,
public:
  Sweep(InputParser::ParsedInput input);

  void run(Configuration& configuration, const Lattice& lattice);

  template<typename HamiltonianType>
  void loopOverGroups(Configuration& configuration, const Lattice& lattice,
      const HamiltonianType& hamiltonian) {
    std::map<double, Bond> bonds = configuration.getBonds();
    std::vector<double> tauGroupStarts = configuration.getTauGroupStarts();

    int groupNum = 0;
    auto currIter = bonds.begin();
    while (currIter != bonds.end()) {
      if (groupNum + 1 == numTimeGroups || 
          currIter->first >= configuration.getTauGroupStarts()[groupNum + 1]) { // When the if executes, we have entered a new time group
        runGroupUpdates(configuration, lattice, hamiltonian, groupNum);
        if (groupNum + 1 == numTimeGroups) break; // This ensures we break out before currIter gets to bonds.end()
        groupNum++;
      }
      currIter++; 
    }
  }

  template<typename HamiltonianType>
  void runGroupUpdates(Configuration& configuration, const Lattice& lattice,
      const HamiltonianType& hamiltonian, int groupNum) {
    // Get the time bounds for the group
    std::vector<double> tauGroupStarts = configuration.getTauGroupStarts();
    double lowerBound = tauGroupStarts[groupNum];
    double upperBound = groupNum + 1 == tauGroupStarts.size() ? 
        configuration.getBeta() : tauGroupStarts[groupNum + 1];
    std::pair<double, double> groupBounds(lowerBound, upperBound); 

    // Create Update classes and call their run functions //TODO: Run in parallel?
    for (int i = 0; i < numUpdatesPerGroup; i++) {
      Update<HamiltonianType> update(lowerBound, upperBound);
      update.run(configuration, lattice, hamiltonian);
    }
  }

private: 
  InputParser::ParsedInput input;

  // Read in members
  int initNumTimeGroups;
  int numTimeGroups;
  double scaleNumUpdates;

  // Calculated members
  int numUpdatesPerGroup;
  
  // Final data members
  std::vector<int> finNumBondsPerGroup; //NOTE: The length of this vector will be equal to initNumTimeGroups, not finNumTimeGroups, b/c in order to report the number of bonds in each time group, you have to use the same time groups you just used.
  std::map<int, int> finNumBondsPerType;
  int finNumTimeGroups;
  int finNumAccepts;
  int finNumRejects;
  int finNumInserts;
  int finNumRemoves;
  int finNumBonds;

};