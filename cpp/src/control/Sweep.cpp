#include <iostream>

#include "Sweep.hpp"


int calcNumUpdatesPerGroup(double scale, double beta, int numSites, 
    int numTimeGroups);


Sweep::Sweep(InputParser::ParsedInput input) { //TODO: Just pass in controlInput?
  ControlInput controlInput = input.controlInput;

  this->input = input;
  this->initNumTimeGroups = controlInput.initNumTimeGroups;
  this->scaleNumUpdates = controlInput.scaleNumUpdates;
}


void Sweep::run(Configuration& configuration, const Lattice& lattice) {
  // Calculate the number of time groups
  numTimeGroups = configuration.calcNumTimeGroups(initNumTimeGroups);

  // Create the new vector of tau starts, and save to configuration
  std::vector<double> newTauGroupStarts = generateTauGroupStarts( //TODO: CHANGE THIS TO A MEMBER FUNCTION
      configuration.getBeta(), numTimeGroups);
  configuration.setTauGroupStarts(newTauGroupStarts);

  // Calculate the number of updates per group per sweep to do
  numUpdatesPerGroup = calcNumUpdatesPerGroup(scaleNumUpdates, 
      configuration.getBeta(), lattice.getNumSites(consts::DirsType::X), //TODO: In the future, getNumSites should automatically detect the lattice structure, and use its points to compute total sites
      numTimeGroups);

  // Create the correct hamiltonian object
  consts::HamilModel hamilModel = input.hamiltonianInput.model;
  switch (hamilModel) {
    case consts::HamilModel::RANDOM:
    {
      Random hamiltonian = Random(input);
      executeGroupUpdates(configuration, lattice, hamiltonian);
      break;
    }
    case consts::HamilModel::TVModel:
    {
      TVModel hamiltonian = TVModel(input);
      executeGroupUpdates(configuration, lattice, hamiltonian);
      break;
    }
  }

  //TODO: Store Update object and relevant loop data.
}


int calcNumUpdatesPerGroup(double scale, double beta, 
    int numSites, int numTimeGroups) {
  double width = beta  / numTimeGroups;
  return scale * width * numSites;
}
