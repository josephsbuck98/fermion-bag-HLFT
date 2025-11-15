#pragma once

#include "Configuration.hpp"
#include "InputParser.hpp"
#include "LatticeBase.hpp"


class TVModel { //TODO: CREATE A GENERAL HAMILTONIAN CLASS THAT HANDLES THE SIMILIARITIES BETWEEN THE DIFFERENT HAMILTONIAN TYPES
public:
  TVModel(InputParser::ParsedInput input);
  void normalizeBondTypeProps();

  consts::BondActionType applyUpdate(Configuration& configuration,
    int groupNum, Configuration::RegionData& regionData, 
    const LatticeBase* lattice) const;

  void handleInsert(Configuration& configuration, int groupNum, 
      Configuration::RegionData& regionData, const LatticeBase* lattice) const;

  void handleRemoval(Configuration& configuration, 
      Configuration::RegionData& regionData) const;
  
  double getAcceptProb(consts::BondActionType actionType, int numSites,
      consts::BoundType boundType, double tauGroupWidth,
      int numBondsInRegion) const;
  
  double getBondSelectionProb(int numSites, consts::BoundType boundType) const;

private:
  double acceptProb = 0.5;
  double insertProb = 0.5;
  std::map<int, double> bondTypeProps = {
    {1, 1}, {2, 1}, {3, 0}, {4, 0}, {5, 0}
  };
};