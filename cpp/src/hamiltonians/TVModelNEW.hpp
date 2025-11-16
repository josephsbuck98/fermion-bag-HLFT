#pragma once

#include "HamiltonianBase.hpp"


class TVModelNEW : public HamiltonianBase { //TODO: CREATE A GENERAL HAMILTONIAN CLASS THAT HANDLES THE SIMILIARITIES BETWEEN THE DIFFERENT HAMILTONIAN TYPES
public:
  TVModelNEW(InputParser::ParsedInput input) : HamiltonianBase(input) {};
    
protected:
  double getWeightFactor() const override;

private:

};