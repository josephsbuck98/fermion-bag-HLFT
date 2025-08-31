#include "Random.hpp"


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
  //TODO: Handle trying to insert an already existing key? Just add/subtract tolerance repeatedly until a free space is found?
  
  //TODO: Using acceptProb, randomly choose true or false for accepting the proposal
  //TODO: If accepted, use insertProb to randomly choose true for insert, or false for delete.
  //TODO: If inserting, use bondTypeProps to choose a bond size. Use bounds and uniform distribution to choose
  //TODO:     a random tau. Insert into the map, and handle preexisting key. If it can be inserted close enough 
  //TODO:     to its original random tau, choose a new random tau. Repeat until success.
  //TODO: If deleting, get a vector of bonds currently within the time group. Randomly choose an index using
  //TODO:     a uniform distribution. Delete it from the map. 
  //TODO: 
  //TODO: Return enum-ed values indicating rejection, insertion, removal?
}