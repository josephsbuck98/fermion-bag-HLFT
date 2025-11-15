#pragma once

#include "Configuration.hpp"
#include "InputParser.hpp"
#include "LatticeBase.hpp"


class HamiltonianBase {
public:
  HamiltonianBase();
  virtual ~HamiltonianBase() = default;

  consts::BondActionType applyUpdate(Configuration& configuration, 
      int groupNum, Configuration::RegionData& regionData, 
      const LatticeBase* lattice) const;


protected:
  double acceptProb = 0.5;
  double insertProb = 0.5;
  std::map<int, double> bondTypeProps = {
    {1, 1}, {2, 1}, {3, 0}, {4, 0}, {5, 0}
  };

  double getAcceptProb(consts::BondActionType actionType, int numSites, 
    consts::BoundType boundType, double tauGroupWidth, 
    int numBondsInRegion) const;

  virtual double getWeightFactor() const = 0;


private:
  void normalizeBondTypeProps();

  void handleInsert(Configuration& configuration, int groupNum, 
    Configuration::RegionData& regionData, const LatticeBase* lattice) const;

  void handleRemoval(Configuration& configuration, 
      Configuration::RegionData& regionData) const;

  double getBondSelectionProb(int numSites, consts::BoundType boundType) const;
  
};