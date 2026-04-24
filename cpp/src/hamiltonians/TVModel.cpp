#include "TVModel.hpp"

#include <unsupported/Eigen/MatrixFunctions>


double TVModel::getWeightFactor(const Configuration& configuration, 
    consts::BondActionType actionType, std::pair<double, int> tauToInsRem, 
    const Bond& newBond) const {
  //TODO: Handle choice of algorithm here
  if (algo == consts::TVAlgo::GREEN) {
    return getWeightFactor_green(configuration, actionType, tauToInsRem, newBond);
  } else if (algo == consts::TVAlgo::GREEN_FAST) {
    return 0.0;
  } else if (algo == consts::TVAlgo::BRUTE) {
    return getWeightFactor_brute(configuration, actionType, tauToInsRem, newBond);
  } else {
    std::cout << "TVAlgo not recognized. Aborting." << std::endl;
    throw std::runtime_error("TVAlgo not recognized. Aborting.");
  }
}


double TVModel::getWeightFactor_brute(const Configuration& configuration, 
    consts::BondActionType actionType, std::pair<double, int> tauToInsRem, 
    const Bond& newBond) const {
  double defaultTau = -1.0; Bond defaultBond = Bond({});
  double Wk = computeW(configuration, defaultTau, defaultBond);

  //TODO: Clean-up, scale back, and formalize printouts
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


double TVModel::getWeightFactor_green(const Configuration& configuration, 
    consts::BondActionType actionType, std::pair<double, int> tauToInsRem,
    const Bond& newBond) const {

  if (tauToInsRem.first < 0) return 0.0;

  bool remove;
  double factor;
  Bond extraBond = newBond;
  
  if (actionType == consts::BondActionType::INSERTION) {
    remove = false;
    factor = omega;
  } else if (actionType == consts::BondActionType::REMOVAL) {
    remove = true;
    extraBond = configuration.getBond(tauToInsRem.first);
    factor = 1 / omega;
  }

  Eigen::MatrixXd invArg = configuration.getHProd_Wrap(cosh2alpha, sinh2alpha, 
      tauToInsRem.first, remove);
  // Return 0 if it is a removal from an empty configuration
  if (invArg.cols() == 0) return 0.0;
  
  invArg.diagonal().array() += 1.0;
  Eigen::MatrixXd G = invArg.inverse();

  auto& bondSites = extraBond.getSites();
  std::vector<const SiteBase*> bondSitesVec(bondSites.begin(), bondSites.end());
  Eigen::MatrixXd bondMat = configuration.genMatForBond(G.cols(), bondSitesVec, 
      cosh2alpha, sinh2alpha);

  Eigen::MatrixXd I = Eigen::MatrixXd::Identity(G.cols(), G.cols());
  Eigen::MatrixXd detArg = I + ((I - G) * (bondMat - I));
  return factor * detArg.determinant();
}


double TVModel::computeW(const Configuration& configuration, double tau, 
    const Bond& bond) const {
  Eigen::MatrixXd detArg = configuration.getHProd_noWrap(omega, cosh2alpha, 
      sinh2alpha, tau, bond);
  detArg.diagonal().array() += 1.0;
  double det = detArg.determinant();

  if (det < 0) {
    std::cout << "DETERMINANT " << det << " IS NEGATIVE!!!!!" << std::endl;
    std::cout << "detArg: " << std::endl;
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
    std::cout << "DETERMINANT " << det << " IS NEGATIVE!!!!!" << std::endl;
    throw std::runtime_error("Determinant of 1+Prods[exp(h_b)] was negative.");
  }

  return det;
}


void TVModel::computeOmega() {
  if (V == 0) {
    omega = 1.0e8;
    std::cout << "TVModel: WARNING. The V parameter is 0, meaning the omega\n";
    std::cout << "  parameter is large and positive. A value of 1.0e8 is\n";
    std::cout << "  being imposed." << std::endl;
    return;
  }
  omega = t * t / V / (1 - (V / (2 * t)) * (V / (2 * t)));
}


void TVModel::computeSinh2alpha() {
  sinh2alpha = V / t / (1 - (V / (2 * t)) * (V / (2 * t)));
}


void TVModel::computeCosh2alpha() {
  cosh2alpha = (1 + (V / (2 * t)) * (V / (2 * t))) / (1 - (V / (2 * t)) * (V / (2 * t)));
}