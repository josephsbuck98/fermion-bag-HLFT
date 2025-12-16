#include "TVModelNEW.hpp"

#include <unsupported/Eigen/MatrixFunctions>


double TVModelNEW::getWeightFactor(const Configuration& configuration, 
    consts::BondActionType actionType, std::pair<double, int> tauToInsRem, 
    const Bond& newBond) const {
  




  std::cout << "Starting Update Analysis: " << std::endl;
  std::cout << "Nbonds: " << configuration.getNumBonds() << std::endl;
  std::cout << "omega: " << omega << std::endl;



  double Wk = computeWk(configuration);




  std::cout << "Wk: " << Wk << std::endl;





  if (actionType == consts::BondActionType::INSERTION) {



    std::cout << "INSERTING " << tauToInsRem.first << "..." << std::endl;
    std::cout <<  newBond << std::endl;


    
    double Wkp1 = computeWkp1(configuration, tauToInsRem, newBond);



    std::cout << "Wkp1: " << Wkp1 << std::endl;
    std::cout << "R: " << omega * Wkp1 / Wk << std::endl;



    return omega * Wkp1 / Wk;
  } else if (actionType == consts::BondActionType::REMOVAL) {



    std::cout << "REMOVING " << tauToInsRem.first << "..." << std::endl;



    double Wkm1 = computeWkm1(configuration, tauToInsRem);





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




  std::cout << "Argument of Determinant: " << std::endl;
  std::cout << std::setprecision(10);




  for (int i = 0; i < detArg.rows(); ++i) {
    for (int j = 0; j < detArg.rows(); ++j) {
      std::cout << std::setw(15) << detArg(i, j) << " ";
    }
    std::cout << "\n";
  }

  double det = detArg.determinant();

  if (det < -1) {
    std::cout << "DETERMINANT " << det << " IS NEGATIVE!!!!!" << std::endl;
    std::cout << configuration << std::endl; 


    exit(1);
  }

  return det;
}


double TVModelNEW::computeWk(const Configuration& configuration) const {
  int nBonds = configuration.getNumBonds();
  double defaultTau = -1.0; Bond defaultBond = Bond({-1});
  double Wk = computeW(configuration, defaultTau, defaultBond);
  return Wk;
}


double TVModelNEW::computeWkp1(const Configuration& configuration, 
    std::pair<double, int> tauToInsert, const Bond& bondToInsert) const {
  int nBonds = configuration.getNumBonds();
  double Wkp1 = computeW(configuration, tauToInsert.first, bondToInsert);
  return Wkp1;
}


double TVModelNEW::computeWkm1(const Configuration& configuration,
    std::pair<double, int> tauToRemove) const {
  int nBonds = configuration.getNumBonds();
  Bond defaultBond = Bond({-1});
  double Wkm1 = computeW(configuration, tauToRemove.first, defaultBond);
  return Wkm1;
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