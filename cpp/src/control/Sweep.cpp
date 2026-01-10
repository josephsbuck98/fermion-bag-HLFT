#include <iostream>

#include "Sweep.hpp"


int calcNumUpdatesPerGroup(double scale, double beta, int numSites, 
    int numTimeGroups);


Sweep::Sweep(InputParser::ParsedInput input, int id) { 
  ControlInput controlInput = input.controlInput;

  this->input = input;
  this->numTimeGroups = controlInput.numTimeGroups;
  this->scaleNumUpdates = controlInput.scaleNumUpdates;
  this->id = id;
}


void Sweep::run(Configuration& configuration, const LatticeBase* lattice) {
  // // Calculate the number of time groups and create the new vector of group starts
  // numTimeGroups = configuration.calcNumTimeGroups(numTimeGroups);
  // std::vector<double> newTauGroupStarts = generateTauGroupStarts( //TODO: CHANGE THIS TO A MEMBER FUNCTION
  //     configuration.getBeta(), numTimeGroups);
  // configuration.setTauGroupStarts(newTauGroupStarts); //TODO: UNCOMMENT TO UPDATE TAU GROUP STARTS

  // Calculate the number of updates per group per sweep to do
  //TODO: If we keep the number of time groups constant, this function does not
  //TODO: need to be run for every sweep
  numUpdatesPerGroup = calcNumUpdatesPerGroup(scaleNumUpdates, 
      configuration.getBeta(), lattice->getTotNumSites(),
      this->numTimeGroups);

  // Create the correct hamiltonian object
  consts::HamilModel hamilModel = input.hamiltonianInput.model;
  switch (hamilModel) {
    case consts::HamilModel::RANDOM:
    {
      Random hamiltonian = Random(input);
      executeGroupUpdates(configuration, hamiltonian, lattice);
      break;
    }
    case consts::HamilModel::TVModel:
    {
      TVModel hamiltonian = TVModel(input);
      executeGroupUpdates(configuration, hamiltonian, lattice);
      break;
    }
  }

}


int calcNumUpdatesPerGroup(double scale, double beta, 
    int numSites, int numTimeGroups) {
  double width = beta  / numTimeGroups;
  int numUpdates = static_cast<int>(std::ceil(scale * width * numSites));
  //TODO: IS THIS ARTIFICIALLY INCREASING MEAN NBONDS AT LOW BETA?
  //TODO: numUpdatees = number of unique bond possibilities
  return numUpdates < 1 ? 1 : numUpdates;
}
