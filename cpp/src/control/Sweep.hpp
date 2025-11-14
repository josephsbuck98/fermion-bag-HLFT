#pragma once

#include <map>
#include <vector>

#include "Configuration.hpp"
#include "InputParser.hpp"
#include "Update.hpp"


class Sweep { 
public:
  Sweep(InputParser::ParsedInput input, int id);

  void run(Configuration& configuration, const LatticeBase* lattice);

  template<typename HamiltonianType>
  void executeGroupUpdates(Configuration& configuration,
      const HamiltonianType& hamiltonian, const LatticeBase* lattice) {

    std::vector<double> tauGroupStarts = configuration.getTauGroupStarts();
    
    // Loop over each time group
    for (int groupNum = 0; groupNum < tauGroupStarts.size(); groupNum++) {
      // Get the time bounds for the group and update regionData
      double lowerBound = tauGroupStarts[groupNum];
      double upperBound = groupNum == tauGroupStarts.size() - 1 ? 
          configuration.getBeta() : tauGroupStarts[groupNum + 1];  
      Configuration::RegionData regionData{lowerBound, upperBound, {}, {}, 0};
      
      const std::set<std::pair<double, int>>& taus = configuration.getTaus();
      regionData.computeIterators(taus);

      // Create Update classes and call their run functions 
      for (int i = 0; i < numUpdatesPerGroup; i++) {
        Update<HamiltonianType> update(groupNum, regionData);
        auto updateType = update.run(configuration, hamiltonian, lattice);
        switch (updateType) {
          case consts::BondActionType::REJECTION:
            finNumRejects++;
            break;
          case consts::BondActionType::INSERTION:
            finNumInserts++;
            regionData.nBondsInRegion++;
            if ((regionData.nBondsInRegion == 1) || // Executes if this is the first bond being added to the tau group
                (regionData.itLow != taus.begin() && // Executes if an element smaller than that pointed to be itLow was inserted
                (*std::prev(regionData.itLow)).first >= regionData.lower)) { 
              regionData.computeIterators(taus);
            }
            break;
          case consts::BondActionType::REMOVAL:
            finNumRemoves++;
            regionData.nBondsInRegion--;
            if (taus.find(*regionData.itLow) == taus.end()) {
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