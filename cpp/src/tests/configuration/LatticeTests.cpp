#include <cassert>
#include <iostream>
#include <string>

#include "Lattice.hpp"

void testLimsKeys();
void testNptsKeys();
void testValidBounds();
void testValidNpts();
void testFiveSiteIntegerLattice();

void runLatticeTests() {
  std::cout << ">>>>>Running LatticeTests...\n";

  // Tests various input combos that are not valid
  // 1. xlims present, ynpts present (xnpts not present)
  // 2. ylims present, ynpts present (xnpts not present)
  // 3. xlims & xnpts present, but lower bound greater than upper bound
  // 4. xlims & xnpts present, but xnpts is negative
  // 5. Valid input params. xlims 1 to 5, 5 points. Use [] to verify 1 2 3 4 5. 
  testLimsKeys();
  testNptsKeys();
  testValidBounds();
  testValidNpts();
  testFiveSiteIntegerLattice();
  // Test that valid input results in expected lattice

  std::cout << ">>>>>Completed LatticeTests.\n\n\n";
};

void testLimsKeys() {
  std::map<std::string, std::pair<float, float>> lims;
  std::map<std::string, int> npts;

  std::cout << "Testing for correct errors for invalid lims keys..." << std::endl; 

  lims["ylims"] = {1.00, 5.00};
  npts["xnpts"] = 5;

  //TODO: Finish this function once you can get the keys from the lims and npts map
  //Lattice l(lims, npts);

  std::cout << std::endl;

};

void testNptsKeys() {
  std::cout << "Testing for correct errors for invalid npts keys..." << std::endl;

  std::cout << std::endl;

};

void testValidBounds() {
  std::map<std::string, std::pair<float, float>> lims;
  std::map<std::string, int> npts;

  std::cout << "Testing invalid bounds..." << std::endl;

  lims["xlims"] = {5.00, 1.00};
  npts["xnpts"] = 5;
  
  try {
    Lattice l(lims, npts);
    assert(false);
  } catch (std::invalid_argument ex) {
    assert(true);
    //assert(ex.what().includes("greater"));
  };

  std::cout << std::endl;

};

void testValidNpts() {
  std::map<std::string, std::pair<float, float>> lims;
  std::map<std::string, int> npts;

  std::cout << "Testing invalid npts value..." << std::endl;

  lims["xlims"] = {1.00, 5.00};
  npts["xnpts"] = -5;

  try {
    Lattice l(lims, npts);
    assert(false);
  } catch (std::invalid_argument ex) {
    assert(true);
  };

  std::cout << std::endl;

};

void testFiveSiteIntegerLattice() {
  std::map<std::string, std::pair<float, float>> lims;
  std::map<std::string, int> npts;

  std::cout << "Testing simple lattice construction..." << std::endl;

  lims["xlims"] = {1.00, 5.00};
  npts["xnpts"] = 5;

  Lattice l(lims, npts);

  assert(l.getNumSites() == npts["xnpts"]);
  for (int i = 0; i < l.getNumSites(); i++) {
    assert(l[i] == float(i + 1));
  };

  std::cout << std::endl;

};
