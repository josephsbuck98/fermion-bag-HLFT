#pragma once

#include <map>
#include <vector>

#include "Configuration.hpp"
#include "InputParser.hpp"
#include "Lattice.hpp"
#include "Update.hpp"

class Sweep { //This is the only class that will both define the loop structure AND handle the majority of the inner workings of each iteration. It will loop over the different time groupings, and within each grouping it will loop for a specified number of times, calling update each iteration.,
public:
  Sweep(InputParser::ParsedInput input);

  void run(Configuration& configuration, const Lattice& lattice);

  template<typename HamiltonianType>
  void executeGroupUpdates(Configuration& configuration, const Lattice& lattice,
      const HamiltonianType& hamiltonian) {

    std::vector<double> tauGroupStarts = configuration.getTauGroupStarts();
    // Loop over each time group
    for (int groupNum = 1; groupNum <= tauGroupStarts.size(); groupNum++) {
      // Get the time bounds for the group
      double lowerBound = tauGroupStarts[groupNum - 1];
      double upperBound = groupNum == tauGroupStarts.size() ? 
          configuration.getBeta() : tauGroupStarts[groupNum];

      // Create Update classes and call their run functions 
      for (int i = 0; i < numUpdatesPerGroup; i++) {
        Update<HamiltonianType> update(lowerBound, upperBound);
        auto updateType = update.run(configuration, lattice, hamiltonian);
        switch (updateType) {
          case consts::BondActionType::REJECTION: finNumRejects++; break;
          case consts::BondActionType::INSERTION: finNumInserts++; break;
          case consts::BondActionType::REMOVAL: finNumRemoves++; break;
        }
      }
    }

    finNumBondsPerType = configuration.getBondsPerType();
    finNumTimeGroups = tauGroupStarts.size();
    finNumBonds = configuration.getNumBonds();
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
  std::map<int, int> finNumBondsPerType;
  int finNumTimeGroups = 0;
  int finNumRejects = 0;
  int finNumInserts = 0;
  int finNumRemoves = 0;
  int finNumBonds = 0;

};