#pragma once

#include "HamiltonianBase.hpp"


class TVModelNEW : public HamiltonianBase { //TODO: CREATE A GENERAL HAMILTONIAN CLASS THAT HANDLES THE SIMILIARITIES BETWEEN THE DIFFERENT HAMILTONIAN TYPES
public:
  TVModelNEW(InputParser::ParsedInput input) : HamiltonianBase(input),
      nDims(input.latticeInput.x_nsites), t(input.hamiltonianInput.t), 
      V(input.hamiltonianInput.V) {
    if (t == 0 && V == 0) {
      throw std::runtime_error("TVModel: Both t and V are 0. Undefined "
        "behavior. Aborting.");
    }
    computeOmega();
    computeCosh2alpha();
    computeSinh2alpha();
    // std::cout << std::endl << "t: " << t << ", V: " << V << std::endl;
    // std::cout << std::endl << "omega: " << omega << ", cosh2alpha: " << cosh2alpha << ", sinh2alpha: " << sinh2alpha << std::endl;
  };
    
protected:
  double getWeightFactor(const Configuration& configuration, 
      consts::BondActionType actionType, std::pair<double, int> tauToInsRem, 
      const Bond& newBond) const override;

private:
  int nDims = 0;
  double t = 0.0, V = 0.0;
  double omega, cosh2alpha, sinh2alpha;

  double computeW(const Configuration& configuration, double tau, 
      const Bond& bond) const;

  double computeWk(const Configuration& configuration) const;
  double computeWkp1(const Configuration& configuration, 
    std::pair<double, int> tauToInsert, const Bond& bondToInsert) const;
  double computeWkm1(const Configuration& configuration,
    std::pair<double, int> tauToRemove) const;
  
  void computeCosh2alpha();
  void computeSinh2alpha();
  void computeOmega();
};