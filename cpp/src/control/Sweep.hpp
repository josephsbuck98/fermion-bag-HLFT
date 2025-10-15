#pragma once

#include <map>
#include <vector>

#include "Configuration.hpp"
#include "InputParser.hpp"
#include "Lattice.hpp"
#include "Update.hpp"

class Sweep { 
public:
  Sweep(InputParser::ParsedInput input, int id);

  void run(Configuration& configuration, const Lattice& lattice);

  template<typename HamiltonianType>
  void executeGroupUpdates(Configuration& configuration, const Lattice& lattice,
      const HamiltonianType& hamiltonian) {

    std::set<std::pair<double, int>> taus = configuration.getTaus();
    std::vector<double> tauGroupStarts = configuration.getTauGroupStarts();
    
    // Loop over each time group
    for (int groupNum = 0; groupNum < tauGroupStarts.size(); groupNum++) {
      // Get the time bounds for the group
      double lowerBound = tauGroupStarts[groupNum];
      double upperBound = groupNum == tauGroupStarts.size() - 1 ? 
          configuration.getBeta() : tauGroupStarts[groupNum + 1];
        
      // Get the starting number of bonds in this time group
      Configuration::RegionData regionData{lowerBound, upperBound, {}, {}, 0};
      regionData.computeIterators(taus);

      auto it_low = taus.lower_bound({lowerBound, std::numeric_limits<int>::min()});
      auto it_high = taus.upper_bound({upperBound, std::numeric_limits<int>::max()});
      auto nbr = std::distance(it_low, it_high);
      std::pair<double, int> lowTau = *it_low;
      //TODO: After above changes, does not agree exactly with previous runs. Probably n issue with deleting the lowest iterator in the group?
      //TODO: CONTINUE CONVERTING TO ONLY USING REGION DATA, NOT THESE INDIVIDUAL VARIABLES

      // Create Update classes and call their run functions 
      for (int i = 0; i < numUpdatesPerGroup; i++) {
        Update<HamiltonianType> update(lowerBound, upperBound, groupNum, regionData);
        auto updateType = update.run(configuration, lattice, hamiltonian);
        switch (updateType) {
          case consts::BondActionType::REJECTION:
            finNumRejects++;
            break;
          case consts::BondActionType::INSERTION:
            finNumInserts++;
            regionData.nBondsInRegion++;
            break;
          case consts::BondActionType::REMOVAL:
            finNumRemoves++;
            regionData.nBondsInRegion--;
            if (taus.find(lowTau) != taus.end()) {
              regionData.computeIterators(taus);
            }
            break;
        }
      }
    }

    finNumBondsPerType = configuration.getBondsPerType();
    finNumTimeGroups = tauGroupStarts.size();
    finNumBonds = configuration.getNumBonds();
  }

  std::map<int, int> getNumBondsPerType() const {return finNumBondsPerType;}
  int getFinNumTimeGroups() const {return finNumTimeGroups;}
  int getNumRejects() const {return finNumRejects;}
  int getNumInserts() const {return finNumInserts;}
  int getNumRemoves() const {return finNumRemoves;}
  int getNumBonds() const {return finNumBonds;}
  int getId() const {return id;}

  bool setId(int newId) {id = newId; return true;}

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
  int id = -1;

};