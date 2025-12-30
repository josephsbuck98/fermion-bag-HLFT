#pragma once

#include "Configuration.hpp"
#include "InputParser.hpp"
#include "LatticeBase.hpp"


class HamiltonianBase {
public:
  HamiltonianBase(InputParser::ParsedInput input);
  virtual ~HamiltonianBase() = default;

  consts::BondActionType applyUpdate(Configuration& configuration, 
      int groupNum, Configuration::RegionData& regionData, 
      const LatticeBase* lattice) const;


protected:
  consts::HamilModel type;
  double acceptProb = 0.5;
  double insertProb = 0.5;
  std::map<int, double> bondTypeProps = {
    {1, 1}, {2, 1}, {3, 0}, {4, 0}, {5, 0}
  };

  double getAcceptProb(const Configuration& configuration, 
    consts::BondActionType actionType, double tauGroupWidth, 
    int numBondsInRegion, std::pair<double, int> tauToInsRem, 
    const Bond& newBond) const;

  virtual double getWeightFactor(const Configuration& configuration, 
      consts::BondActionType actionType, std::pair<double, int> tauToInsRem, 
      const Bond& newBond) const = 0;


private:
  void normalizeBondTypeProps();

  void handleInsert(Configuration& configuration, int groupNum, 
    Configuration::RegionData& regionData, const LatticeBase* lattice, 
    std::pair<double, int> tauToInsert, Bond newBond) const;

  void handleRemoval(Configuration& configuration, 
      Configuration::RegionData& regionData,  
      std::pair<double, int> tauToRem) const;

  double getBondSelectionProb() const;

  Bond createBondToInsert(const LatticeBase* lattice) const;
  std::pair<double, int> chooseTauToInsert(
      Configuration::RegionData& regionData, int groupNum) const;
  std::pair<double, int> chooseTauToRemove(
      Configuration::RegionData& regionData) const;
  
};