#pragma once

#include "HamiltonianBase.hpp"


class TVModelNEW : public HamiltonianBase { //TODO: CREATE A GENERAL HAMILTONIAN CLASS THAT HANDLES THE SIMILIARITIES BETWEEN THE DIFFERENT HAMILTONIAN TYPES
public:
  TVModelNEW(InputParser::ParsedInput input) : HamiltonianBase(input) {};
    
protected:
  double getWeightFactor(const Configuration& configuration, 
      consts::BondActionType actionType, std::pair<double, int> tauToInsRem, 
      const Bond& newBond) const override;

private:
  double computeWk(const Configuration& configuration) const;
  double computeWkp1(const Configuration& configuration, 
    std::pair<double, int> tauToInsert, const Bond& bondToInsert) const;
  double computeWkm1(const Configuration& configuration,
    std::pair<double, int> tauToRemove) const;
};