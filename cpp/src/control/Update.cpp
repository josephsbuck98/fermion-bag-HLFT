//TODO: At first, just implement all of the RANDOM Hamiltonian logic here. But
//TODO: in the future, Hamiltonian specific logic will go in hamiltonians, and
//TODO: this will be just a driver for an individual, general update.
#include <iostream>

#include "Update.hpp"


Update::Update() {

}


void run() {
  //TODO: Compute accept/reject with acceptProb. Return if reject.
  //TODO: Compute insert/delete with insertProb. 
  //TODO: If insert
  //TODO:     Select a random tau between bounds of group
  //TODO:     Select bondSize at random with bondTypeProps distribution
  //TODO:     Compute starting lattice index randomly AND using lattice's bc type
  //TODO:     Create new Bond and insert into Configuration.
  //TODO: If delete
  //TODO:     Select a tau at random from the taus in configuration.taus.
  //TODO:     Use that tau to remove the bond. 
}