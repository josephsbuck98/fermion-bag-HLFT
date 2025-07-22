#include <cassert>
#include <iostream>

#include "Bond.hpp"
#include "Configuration.hpp"

void testAddBond(std::array<double, 4> taus, std::array<Bond, 4> bonds);
void testAddBonds(std::array<double, 4> taus, std::array<Bond, 4> bonds);
void testDelBond(std::array<double, 4> taus, std::array<Bond, 4> bonds);
void testGetBond(std::array<double, 4> taus, std::array<Bond, 4> bonds);
void testGetBonds(std::array<double, 4> taus, std::array<Bond, 4> bonds);
void testGetNumBonds(std::array<double, 4> taus, std::array<Bond, 4> bonds);
void testEquality(std::array<double, 4> taus, std::array<Bond, 4> bonds);
void testTruncateToTolerance();

void runConfigurationTests() {
  Bond b1({1}); Bond b2({2, 3}); Bond b3({4, 5, 6}); Bond b4({3, 4});
  std::array<double, 4> taus = {0.01, 0.025, 0.1, 0.45};
  std::array<Bond, 4> bonds = {b1, b2, b3, b4};

  std::cout << ">>>>>Running configuration tests...\n";

  testAddBond(taus, bonds);
  testGetBond(taus, bonds);
  testAddBonds(taus, bonds);
  testTruncateToTolerance();
  testDelBond(taus, bonds);
  testGetBonds(taus, bonds);
  testGetNumBonds(taus, bonds);
  testEquality(taus, bonds);

  std::cout << ">>>>>Completed configuration tests.\n\n";
}

void testAddBond(std::array<double, 4> taus, std::array<Bond, 4> bonds) {
  Configuration c(1e-5);

  // Test adding bonds
  for(int i = 0; i < taus.size(); i++) {
    c.addBond(taus[i], bonds[i]);
    assert(c.getNumBonds() == i + 1);
  }

  // Test adding a bond to a pre-existing tau
  try {
    c.addBond(taus[0], bonds[0]);
    assert(false);
  } catch (const std::runtime_error& err) {
    assert(true);
  }
}

void testAddBonds(std::array<double, 4> taus, std::array<Bond, 4> bonds) {
  Configuration c(1e-5); Configuration cc(1e-5);
  std::vector<double> tausVec(taus.begin(), taus.end());
  std::vector<Bond> bondsVec(bonds.begin(), bonds.end());

  // Test adding multiple bonds at once
  c.addBonds(tausVec, bondsVec);
  for (int i = 0; i < taus.size(); i++) {
    cc.addBond(taus[i], bonds[i]);
  }
  assert(c == cc);

  // Test adding the same bonds again
  for (int i = 0; i < taus.size(); i++) {
    try {
      cc.addBond(taus[i], bonds[i]);
      assert(false);
    } catch (const std::runtime_error& err) {
      assert(true);
    }
  }
}

void testDelBond(std::array<double, 4> taus, std::array<Bond, 4> bonds) {
  //TODO: Test deleting a bond that isn't there. 
  Configuration c(1e-5);
  std::vector<double> tausVec(taus.begin(), taus.end());
  std::vector<Bond> bondsVec(bonds.begin(), bonds.end());
  
  // Test deleting bonds
  c.addBonds(tausVec, bondsVec);
  for(int i = taus.size(); i >= 0; i--) {
    // On first iter, test deleting bond that isn't there
    c.delBond(taus[i]);
    assert(c.getNumBonds() == i); 
  }

  // Test deleting a bond that isn't there
  c.addBonds(tausVec, bondsVec);
  c.delBond(tausVec[0]);
  c.delBond(tausVec[0]);
  assert(c.getNumBonds() == bondsVec.size() - 1);
}

void testGetBond(std::array<double, 4> taus, std::array<Bond, 4> bonds) {
  Configuration c(1e-5);

  // Test regular getBond
  c.addBond(taus[0], bonds[0]);
  Bond b = c.getBond(taus[0]);
  assert(b == bonds[0]);

  // Test getting bond that isn't there
  try {
    Bond bb = c.getBond(taus[1]);
    assert(false);
  } catch (std::out_of_range ex) {
    assert(true);
  }
}

void testGetBonds(std::array<double, 4> taus, std::array<Bond, 4> bonds) {
  Configuration c(1e-5);
  std::vector<double> tausVec(taus.begin(), taus.end());
  std::vector<Bond> bondsVec(bonds.begin(), bonds.end());

  // Test getBonds
  c.addBonds(tausVec, bondsVec);
  std::map<double, Bond> cBonds = c.getBonds();
  for (int i = 0; i < taus.size(); i++) {
    assert(cBonds.at(taus[i]) == bonds[i]);
  }
}

void testGetNumBonds(std::array<double, 4> taus, std::array<Bond, 4> bonds) {
  Configuration c(1e-5);
  std::vector<double> tausVec(taus.begin(), taus.end());
  std::vector<Bond> bondsVec(bonds.begin(), bonds.end());

  // Test getNumBonds
  c.addBonds(tausVec, bondsVec);
  assert(c.getNumBonds() == bondsVec.size());
}

void testEquality(std::array<double, 4> taus, std::array<Bond, 4> bonds) {
  Bond b({3, 4});
  Bond bb({1});
  Configuration c(1e-5); Configuration cc(1e-5);

  // Test equality and inequality operators
  assert(c == cc);
  cc.addBond(0.27, b);
  assert(c != cc);
  cc.delBond(0.27);
  assert(c == cc);
  cc.delBond(taus[0]);
  cc.addBond(0.001, b);
  assert(c != cc);
  cc.delBond(taus[0]);
  cc.addBond(taus[0], bb);
  assert(c != cc);
}

void testTruncateToTolerance() {
  double tau = 2.71401;
  double tauDiff = tau + 2e-5;
  Bond b({1});
  Configuration c(1e-5);

  // Test getting the same bond outside the given tolerance
  c.addBond(tau, b);
  try {
    c.getBond(tauDiff);
    assert(false);
  } catch(...) {
    assert(true);
  }

  // Test getting the same bond within the given tolerance
  tauDiff = tau + 1e-6;
  b = c.getBond(tau);
  Bond bb = c.getBond(tauDiff);
  assert(b == bb);
}