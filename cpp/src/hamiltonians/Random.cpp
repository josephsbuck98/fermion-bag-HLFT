#include <random>
#include <algorithm>

#include "Bond.hpp"
#include "Constants.hpp"
#include "Random.hpp"
#include "RandomHelpers.hpp"


Random::Random(InputParser::ParsedInput input) {
  acceptProb = input.hamiltonianInput.acceptProb;
  insertProb = input.hamiltonianInput.insertProb;
  bondTypeProps = input.hamiltonianInput.bondTypeProps;

  normalizeBondTypeProps();
}


void Random::normalizeBondTypeProps() { 
  //NOTE: Currently, only supports integer bond sizes from 1 to 6. ISNT BONDTYPEPROPS A MAP???
  double total = 0.0;
  for (int i = 1; i <= 6; i++) { //TODO: Use an enum for 6?
    double& val = bondTypeProps[i];
    if (val <= 0.0) val = 0.0;
    total += val;
  }

  // Normalize if total is non-zero
  if (total > 0.0) {
    for (int i = 1; i <= 6; i++) {
      bondTypeProps[i] /= total;
    }
  } else { // Otherwise, set to uniform distribution
    for (int i = 1; i <= 6; i++) {
      bondTypeProps[i] = 1.0 / 6.0;
    }
  }
}


double getAcceptProb(consts::BondActionType actionType, int numSites, 
    double tauGroupWidth, int numBondsInRegion) {
  double prob; //TODO: GENERALIZE TO ANY BOND LENGTH, AND PERIODIC OR OPEN BC'S
  if (actionType == consts::BondActionType::INSERTION) {
    prob = 2 * numSites * tauGroupWidth / numBondsInRegion; //TODO: THIS IS HARDCODED FOR A 1D UNIFORM LATTICE WITH PERIODIC BOUNDARIES AND BOND SIZE OF 2
  } else if (actionType == consts::BondActionType::REMOVAL) {
    prob = numBondsInRegion / (2 * numSites * tauGroupWidth); //TODO: THIS IS HARDCODED FOR A 1D UNIFORM LATTICE WITH PERIODIC BOUNDARIES AND BOND SIZE OF 2
  } 
  return prob > 1.0 ? 1.0 : prob;
}


consts::BondActionType Random::applyUpdate(Configuration& configuration, const Lattice& lattice,
    int groupNum, Configuration::RegionData& regionData, const LatticeBase* latticeNEW) const {





  std::cout << std::endl << "Inside Random.applyUpdate(). " << std::endl;
  std::cout << latticeNEW->getNumSites(consts::DirsType::X) << std::endl;
  latticeNEW->printInfo();
  std::cout << std::endl;





  int numSites = lattice.getNumSites(consts::DirsType::X);
  double tauGroupWidth = regionData.upper - regionData.lower;
  bool insertResult = bernoulli(insertProb); 
  if (insertResult) {
    double acceptInsertProb = getAcceptProb(
        consts::BondActionType::INSERTION, numSites, tauGroupWidth, 
            regionData.nBondsInRegion);
    bool acceptInsert = bernoulli(acceptInsertProb);
    if (acceptInsert) {
      handleInsert(configuration, lattice, groupNum, regionData, latticeNEW);
      return consts::BondActionType::INSERTION;
    }
  } else {
    double acceptRemoveProb = getAcceptProb(
        consts::BondActionType::REMOVAL, numSites, tauGroupWidth, 
            regionData.nBondsInRegion);
    bool acceptRemove = bernoulli(acceptRemoveProb);
    if (acceptRemove) {
      handleRemoval(configuration, regionData);
      return consts::BondActionType::REMOVAL;
    }
  }
  return consts::BondActionType::REJECTION;
}


void Random::handleInsert(Configuration& configuration, const Lattice& lattice,
    int groupNum, Configuration::RegionData& regionData, 
    const LatticeBase* latticeNEW) const {
      
  int bondSize = chooseWeightedRandInt(bondTypeProps);
  std::pair<double, int> tauToInsert =  std::pair<double, int>
      (chooseUnifRandDoubWithBounds(regionData.lower, regionData.upper), 
      groupNum);
  int latticeBondStart;
  int numSites = lattice.getNumSites(consts::DirsType::X);
  if (lattice.getBoundType(consts::DirsType::X) == consts::BoundType::OPEN) { // Open boundary
    latticeBondStart = chooseUnifRandIntWithBounds(0, numSites - bondSize + 1);
  } else { // Periodic boundary
    latticeBondStart = chooseUnifRandIntWithBounds(0, numSites);
  }//TODO: HANDLE MULTIPLE DIMENSIONS AND DIFFERENT LATTICE TYPES
  std::set<int> bondSites;
  for (int i = latticeBondStart; i < latticeBondStart + bondSize; i++) {
    bondSites.insert(i % numSites);
  }
  Bond newBond(bondSites);

  // Ensure bondSize is less than or equal to Nsites
  if (bondSize > lattice.getNumSites(consts::DirsType::X)) {
    throw std::runtime_error("bondSize=" + std::to_string(bondSize) + "is " +
        "larger than Nsites=" + std::to_string(lattice.getNumSites(consts::DirsType::X)));
  }

  // Insert into the bond with retries for duplicate taus
  std::pair<double, int> tauCandidate = tauToInsert; //TODO: PUT THIS INTO THE CONFIGURATION CLASS. TEST IT WELL.
  double tol = configuration.getTolerance();
  int maxAttempts = 100;
  for (int attempts = 0; attempts < maxAttempts; ++attempts) {
    try {
      configuration.addBond(tauCandidate, newBond);
      return;
    } catch(const std::exception& ex) {
      std::pair<double, int> tauUp = 
          std::pair<double, int>(tauCandidate.first + tol, tauCandidate.second);
      if (tauUp.first < regionData.upper) {
        tauCandidate = tauUp;
        continue;
      }
      std::pair<double, int> tauDown = tauCandidate;
      tauDown.first -= tol;
      if (tauDown.first > regionData.lower) {
        tauCandidate = tauDown;
        continue;
      }
      std::string message = "Could not insert tau=" + 
          std::to_string(tauToInsert.first) + " within " +
          "bounds [" + std::to_string(regionData.lower) + ", " +
          std::to_string(regionData.upper) + "] after " + 
          std::to_string(maxAttempts) + " attempts.";
      std::cout << message << std::endl;
      
      throw std::runtime_error(message);
    }
  }
}


void Random::handleRemoval(Configuration& configuration,
    Configuration::RegionData& regionData) const {

  auto it = regionData.itLow;
  int indToDelete = chooseUnifRandIntWithBounds(0, regionData.nBondsInRegion);
  std::advance(it, indToDelete);
  std::pair<double, int> tauToDelete = *it;

  configuration.delBond(tauToDelete);
}