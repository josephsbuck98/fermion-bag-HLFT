#include "TVModelNEW.hpp"


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
  std::cout << "Action type was neither INSERTION nor REMOVAL." << std::endl;
  return 1.0;
}


double TVModelNEW::computeWk(const Configuration& configuration) const {
  return 1.0;
}


double TVModelNEW::computeWkp1(const Configuration& configuration, 
    std::pair<double, int> tauToInsert, const Bond& bondToInsert) const {
  return 1.0;
}


double TVModelNEW::computeWkm1(const Configuration& configuration,
    std::pair<double, int> tauToRemove) const {
  return 1.0;
}