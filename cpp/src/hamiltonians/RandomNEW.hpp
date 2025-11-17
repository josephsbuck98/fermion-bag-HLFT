#pragma once

#include "HamiltonianBase.hpp"


class RandomNEW : public HamiltonianBase {
public:
  RandomNEW(InputParser::ParsedInput input) : HamiltonianBase(input) {};
  
protected:
  double getWeightFactor(const Configuration& configuration, 
      consts::BondActionType actionType, std::pair<double, int> tauToInsRem, 
      const Bond& newBond) const override;

private:

};