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


void Sweep::run(Configuration& configuration, const Lattice& lattice,
    const LatticeBase* latticeNEW) {
  // // Calculate the number of time groups and create the new vector of group starts
  // numTimeGroups = configuration.calcNumTimeGroups(numTimeGroups);
  // std::vector<double> newTauGroupStarts = generateTauGroupStarts( //TODO: CHANGE THIS TO A MEMBER FUNCTION
  //     configuration.getBeta(), numTimeGroups);
  // configuration.setTauGroupStarts(newTauGroupStarts); //TODO: UNCOMMENT TO UPDATE TAU GROUP STARTS





  std::cout << std::endl << "Inside Sweep.run(). " << std::endl;
  std::cout << latticeNEW->getNumSites(consts::DirsType::X) << std::endl;
  latticeNEW->printInfo();
  std::cout << std::endl;





  // Calculate the number of updates per group per sweep to do
  numUpdatesPerGroup = calcNumUpdatesPerGroup(scaleNumUpdates, 
      configuration.getBeta(), latticeNEW->getNumSites(consts::DirsType::X), //TODO: In the future, getNumSites should automatically detect the lattice structure, and use its points to compute total sites
      this->numTimeGroups);

  // Create the correct hamiltonian object
  consts::HamilModel hamilModel = input.hamiltonianInput.model;
  switch (hamilModel) {
    case consts::HamilModel::RANDOM:
    {
      Random hamiltonian = Random(input);
      executeGroupUpdates(configuration, lattice, hamiltonian, latticeNEW);
      break;
    }
    case consts::HamilModel::TVModel:
    {
      TVModel hamiltonian = TVModel(input);
      executeGroupUpdates(configuration, lattice, hamiltonian, latticeNEW);
      break;
    }
  }

}


int calcNumUpdatesPerGroup(double scale, double beta, 
    int numSites, int numTimeGroups) {
  double width = beta  / numTimeGroups;
  int numUpdates = static_cast<int>(std::ceil(scale * width * numSites));
  return numUpdates < 1 ? 1 : numUpdates;
}
