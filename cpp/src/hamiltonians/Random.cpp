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
  //NOTE: Currently, only supports integer bond sizes from 1 to 6. 
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


consts::BondActionType Random::applyUpdate(Configuration& configuration, const Lattice& lattice,
    int groupLowerBound, int groupUpperBound) const {
  
  bool acceptResult = bernoulli(acceptProb);
  if (!acceptResult) return consts::BondActionType::REJECTION;

  bool insertResult = bernoulli(insertProb);
  if (insertResult) {
    handleInsert(configuration, lattice, groupLowerBound, groupUpperBound);
    return consts::BondActionType::INSERTION;
  } else {
    handleRemoval(configuration, groupLowerBound, groupUpperBound);
    return consts::BondActionType::REMOVAL;
  }
}


void Random::handleInsert(Configuration& configuration, const Lattice& lattice,
    int groupLowerBound, int groupUpperBound) const {
      
  int bondSize = chooseWeightedRandInt(bondTypeProps);
  double tauToInsert = chooseUnifRandDoubWithBounds(
      groupLowerBound, groupUpperBound); 
  int latticeBondStart = chooseUnifRandIntWithBounds(0, 
      lattice.getNumSites(consts::DirsType::X) - bondSize);
  std::set<int> bondSites;
  for (int i = latticeBondStart; i < latticeBondStart + bondSize; i++) {
    bondSites.insert(i);
  }
  Bond newBond(bondSites);

  // Ensure bondSize is less than or equal to Nsites
  if (bondSize > lattice.getNumSites(consts::DirsType::X)) {
    throw std::runtime_error("bondSize=" + std::to_string(bondSize) + "is " +
        "larger than Nsites=" + std::to_string(lattice.getNumSites(consts::DirsType::X)));
  }

  // Insert into the bond with retries for duplicate taus
  double tauCandidate = tauToInsert; //TODO: PUT THIS INTO THE CONFIGURATION CLASS
  double tol = configuration.getTolerance();
  int maxAttempts = 100;
  for (int attempts = 0; attempts < maxAttempts; ++attempts) {
    try {
      configuration.addBond(tauCandidate, newBond);
      return;
    } catch(const std::exception& ex) {
      double tauUp = tauCandidate + tol;
      if (tauUp < groupUpperBound) {
        tauCandidate = tauUp;
        continue;
      }
      double tauDown = tauCandidate - tol;
      if (tauDown > groupLowerBound) {
        tauCandidate = tauDown;
        continue;
      }
      std::string message = "Could not insert tau=" + 
          std::to_string(tauToInsert) + " within " +
          "bounds [" + std::to_string(groupLowerBound) + ", " +
          std::to_string(groupUpperBound) + "] after " + 
          std::to_string(maxAttempts) + " attempts.";
      std::cout << message << std::endl;
      throw std::runtime_error(message);
    }
  }
}


void Random::handleRemoval(Configuration& configuration, 
    int groupLowerBound, int groupUpperBound) const {
  std::vector<double> deletableTaus;
  std::set<double> taus = configuration.getTaus();

  auto it =  taus.begin();
  while (it != taus.end()) {
    if (*it > groupLowerBound && *it < groupUpperBound) {
      deletableTaus.push_back(*it);
    }
    ++it;
  }

  if (deletableTaus.empty()) return;
  int indToDelete = chooseUnifRandIntWithBounds(0, deletableTaus.size());
  size_t tauToDelete = deletableTaus[indToDelete];

  try {
    configuration.delBond(tauToDelete);
  } catch(const std::exception& ex) {
    std::cout << "Bond deletion failed: " << ex.what() << std::endl;
  }

}