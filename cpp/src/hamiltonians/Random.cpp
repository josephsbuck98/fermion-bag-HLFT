#include <random>
#include <algorithm>

#include "Constants.cpp"
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


void Random::applyUpdate(Configuration& configuration, const Lattice& lattice,
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
  //TODO: Use bondTypeProps to choose a bond size. Use bounds and uniform distribution to choose
  //TODO:     a random tau. Insert into the map, and handle preexisting key. If it can be inserted close enough 
  //TODO:     to its original random tau, choose a new random tau. Repeat until success.
  //TODO: Handle trying to insert an already existing key? Just add/subtract tolerance repeatedly until a free space is found?

}


void Random::handleRemoval(Configuration& configuration, 
    int groupLowerBound, int groupUpperBound) const {
  std::vector<double> deletableTaus;
  std::set<double> taus = configuration.getTaus();

  auto it =  taus.begin();
  while (it != taus.end()) {
    if (it->first > groupLowerBound && it->first < groupUpperBound) {
      deletableTaus.push_back(it->first);
    }
  }

  if (deletableTaus.empty()) return;
  std::uniform_int_distribution<size_t> dist(0, deletableTaus.size() - 1);
  size_t tauToDelete = deletableTaus[dist(globalRNG())];

  try {
    configuration.delBond(tauToDelete);
  } catch(err) {
    std::cout << "Bond deletion failed.\n";
  }

}