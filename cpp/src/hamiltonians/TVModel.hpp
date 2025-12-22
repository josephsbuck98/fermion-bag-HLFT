#pragma once

#include "HamiltonianBase.hpp"


class TVModel : public HamiltonianBase { 
public:
  TVModel(InputParser::ParsedInput input) : HamiltonianBase(input),
      nDims(input.latticeInput.xNSites), t(input.hamiltonianInput.t), 
      V(input.hamiltonianInput.V) {
    if (t == 0 && V == 0) {
      throw std::runtime_error("TVModel: Both t and V are 0. Undefined "
        "behavior. Aborting.");
    }
    computeOmega();
    computeCosh2alpha();
    computeSinh2alpha();

    std::cout << std::setprecision(10);
    std::cout << std::endl << "t: " << t << ", V: " << V << std::endl;
    std::cout << std::endl << "omega: " << omega << ", cosh2alpha: " << cosh2alpha << ", sinh2alpha: " << sinh2alpha << std::endl;
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
  
  void computeCosh2alpha();
  void computeSinh2alpha();
  void computeOmega();
};