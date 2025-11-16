#pragma once

#include "HamiltonianBase.hpp"


class RandomNEW : public HamiltonianBase {
public:
  RandomNEW(InputParser::ParsedInput input) : HamiltonianBase(input) {};
  
protected:
  double getWeightFactor() const override;

private:

};