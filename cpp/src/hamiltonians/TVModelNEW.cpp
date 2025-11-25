#include "TVModelNEW.hpp"

#include <unsupported/Eigen/MatrixFunctions>


double TVModelNEW::getWeightFactor(const Configuration& configuration, 
    consts::BondActionType actionType, std::pair<double, int> tauToInsRem, 
    const Bond& newBond) const {
  double Wk = computeWk(configuration);
  if (actionType == consts::BondActionType::INSERTION) {
    double Wkp1 = computeWkp1(configuration, tauToInsRem, newBond);
    return Wkp1 / Wk;
  } else if (actionType == consts::BondActionType::REMOVAL) {
    double Wkm1 = computeWkm1(configuration, tauToInsRem);
    return Wk / Wkm1;
  } 
  return 1.0;
}


double TVModelNEW::computeW(const Configuration& configuration, double tau, 
    const Bond& bond) const {
  Eigen::MatrixXd hProdMat = configuration.getHProd(nDims, omega, cosh2alpha, 
      sinh2alpha, tau, bond);
  Eigen::MatrixXd detArg = hProdMat.exp();
  detArg.diagonal().array() += 1.0;
  return detArg.determinant();
}


double TVModelNEW::computeWk(const Configuration& configuration) const {
  int nBonds = configuration.getNumBonds();
  double defaultTau = -1.0; Bond defaultBond = Bond({-1});
  double Wk = computeW(configuration, defaultTau, defaultBond);
  Wk *= std::pow(omega, nBonds);
  return 1.0;
}


double TVModelNEW::computeWkp1(const Configuration& configuration, 
    std::pair<double, int> tauToInsert, const Bond& bondToInsert) const {
  int nBonds = configuration.getNumBonds();
  double Wkp1 = computeW(configuration, tauToInsert.first, bondToInsert);
  Wkp1 *= std::pow(omega, nBonds + 1);
  return 1.0;
}


double TVModelNEW::computeWkm1(const Configuration& configuration,
    std::pair<double, int> tauToRemove) const {
  int nBonds = configuration.getNumBonds();
  Bond defaultBond = Bond({-1});
  double Wkm1 = computeW(configuration, tauToRemove.first, defaultBond);
  Wkm1 *= std::pow(omega, nBonds - 1);
  return 1.0;
}


void TVModelNEW::computeOmega() {
  if (V == 0) {
    omega = 1.0e8;
    std::cout << "TVModel: WARNING. The V parameter is 0, meaning the omega\n";
    std::cout << "  parameter is large and positive. A value of 1.0e8 is\n";
    std::cout << "  being imposed." << std::endl;
    return;
  }
  omega = t * t / V - V / 4.0;
}


void TVModelNEW::computeCosh2alpha() {
  cosh2alpha = 4.0 * t * V / (4.0 * t * t - V * V);
}


void TVModelNEW::computeSinh2alpha() {
  sinh2alpha = (4.0 * t * t + V * V) / (4.0 * t * t - V * V);
}