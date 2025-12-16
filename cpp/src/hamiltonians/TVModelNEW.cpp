#include "TVModelNEW.hpp"

#include <unsupported/Eigen/MatrixFunctions>


double TVModelNEW::getWeightFactor(const Configuration& configuration, 
    consts::BondActionType actionType, std::pair<double, int> tauToInsRem, 
    const Bond& newBond) const {
  double defaultTau = -1.0; Bond defaultBond = Bond({-1});
  double Wk = computeW(configuration, defaultTau, defaultBond);

  std::cout << "Update Analysis: " << std::endl;
  std::cout << "Nbonds: " << configuration.getNumBonds() << std::endl;
  std::cout << "Wk: " << Wk << std::endl;

  if (actionType == consts::BondActionType::INSERTION) {
    double Wkp1 = computeW(configuration, tauToInsRem.first, newBond);

    std::cout << "INSERTING " << tauToInsRem.first << "..." << std::endl;
    std::cout <<  newBond << std::endl;
    std::cout << "Wkp1: " << Wkp1 << std::endl;
    std::cout << "R: " << omega * Wkp1 / Wk << std::endl;

    return omega * Wkp1 / Wk;
  } else if (actionType == consts::BondActionType::REMOVAL) {
    double Wkm1 = computeW(configuration, tauToInsRem.first, defaultBond);

    std::cout << "REMOVING " << tauToInsRem.first << "..." << std::endl;
    std::cout << "Wkm1: " << Wkm1 << std::endl;
    std::cout << "R: " << Wkm1 / Wk / omega << std::endl;

    return Wk / Wkm1 / omega;
  } 
  return 1.0;
}


double TVModelNEW::computeW(const Configuration& configuration, double tau, 
    const Bond& bond) const {
  Eigen::MatrixXd detArg = configuration.getHProd(nDims, omega, cosh2alpha, 
      sinh2alpha, tau, bond);
  detArg.diagonal().array() += 1.0;
  double det = detArg.determinant();

  if (det < 0) {
    std::cout << "DETERMINANT " << det << " IS NEGATIVE!!!!!" << std::endl;
    std::cout << "detArg: " << std::endl;
    //TODO: Print detArg here
    for (int i = 0; i < detArg.rows(); i++) {
      std::cout << "  ";
      for (int j = 0; j < detArg.cols(); j++) {
        std::cout << detArg(i, j) << " ";
      }
      std::cout << std::endl;
    }
    std::cout << "tau: " << tau << std::endl;
    std::cout << "bond: " << std::endl << bond << std::endl;
    std::cout << configuration << std::endl;
    throw std::runtime_error("Determinant of 1+Prods[exp(h_b)] was negative.");
  }

  return det;
}


void TVModelNEW::computeOmega() {
  if (V == 0) {
    omega = 1.0e8;
    std::cout << "TVModel: WARNING. The V parameter is 0, meaning the omega\n";
    std::cout << "  parameter is large and positive. A value of 1.0e8 is\n";
    std::cout << "  being imposed." << std::endl;
    return;
  }
  omega = t * t / V / (1 - (V / (2 * t)) * (V / (2 * t)));
}


void TVModelNEW::computeSinh2alpha() {
  sinh2alpha = V / t / (1 - (V / (2 * t)) * (V / (2 * t)));
}


void TVModelNEW::computeCosh2alpha() {
  cosh2alpha = (1 + (V / (2 * t)) * (V / (2 * t))) / (1 - (V / (2 * t)) * (V / (2 * t)));
}