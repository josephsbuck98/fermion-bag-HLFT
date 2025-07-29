#include <cassert>
#include <iostream>

#include "Bond.hpp"
#include "Configuration.hpp"

void testAddBond(std::array<double, 4> taus, std::array<Bond, 4> bonds);
void testAddBonds(std::array<double, 4> taus, std::array<Bond, 4> bonds);
void testDelBond(std::array<double, 4> taus, std::array<Bond, 4> bonds);
void testDelBonds(std::array<double, 4> taus, std::array<Bond, 4> bonds);
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
  testDelBonds(taus, bonds);
  testGetBonds(taus, bonds);
  testGetNumBonds(taus, bonds);
  testEquality(taus, bonds);

  std::cout << ">>>>>Completed configuration tests.\n\n\n";
}

void testAddBond(std::array<double, 4> taus, std::array<Bond, 4> bonds) {
  Configuration c(1e-5);

  std::cout << "Testing addBond..." << std::endl;

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

  std::cout << std::endl;

}

void testAddBonds(std::array<double, 4> taus, std::array<Bond, 4> bonds) {
  Configuration c(1e-5); Configuration cc(1e-5);
  std::vector<double> tausVec(taus.begin(), taus.end());
  std::vector<Bond> bondsVec(bonds.begin(), bonds.end());

  std::cout << "Testing addBonds..." << std::endl;

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

  std::cout << std::endl;

}

void testDelBond(std::array<double, 4> taus, std::array<Bond, 4> bonds) {
  Configuration c(1e-5);
  std::vector<double> tausVec(taus.begin(), taus.end());
  std::vector<Bond> bondsVec(bonds.begin(), bonds.end());
  
  std::cout << "Testing delBond..." << std::endl;

  // Test deleting bonds
  c.addBonds(tausVec, bondsVec);
  for(int i = taus.size(); i >= 0; i--) {
    // On first iter, test deleting bond that isn't there
    if (i == taus.size()) {
      c.delBond(0.00000);
    } else {
      c.delBond(taus[i]);
    }
    assert(c.getNumBonds() == i); 
  }

  std::cout << std::endl;

}

void testDelBonds(std::array<double, 4> taus, std::array<Bond, 4> bonds) {
  Configuration c(1e-5);
  std::vector<double> tausVec(taus.begin(), taus.end());
  std::vector<Bond> bondsVec(bonds.begin(), bonds.end());

  std::cout << "Testing delBonds..." << std::endl;

  c.addBonds(tausVec, bondsVec);
  c.delBonds();
  assert(c.getNumBonds() == 0);

  std::cout << std::endl;

}

void testGetBond(std::array<double, 4> taus, std::array<Bond, 4> bonds) {
  Configuration c(1e-5);

  std::cout << "Testing getBond..." << std::endl;

  // Test regular getBond
  c.addBond(taus[0], bonds[0]);
  Bond b = c.getBond(taus[0]);
  assert(b == bonds[0]);

  // Test getting bond that isn't there
  Bond bb = c.getBond(taus[1]);
  assert(bb.getNumSites() == 0);

  std::cout << std::endl;

}

void testGetBonds(std::array<double, 4> taus, std::array<Bond, 4> bonds) {
  Configuration c(1e-5);
  std::vector<double> tausVec(taus.begin(), taus.end());
  std::vector<Bond> bondsVec(bonds.begin(), bonds.end());

  std::cout << "Testing getBonds..." << std::endl;

  // Test getBonds
  c.addBonds(tausVec, bondsVec);
  std::map<double, Bond> cBonds = c.getBonds();
  for (int i = 0; i < taus.size(); i++) {
    assert(cBonds.at(taus[i]) == bonds[i]);
  }

  std::cout << std::endl;

}

void testGetNumBonds(std::array<double, 4> taus, std::array<Bond, 4> bonds) {
  Configuration c(1e-5);
  std::vector<double> tausVec(taus.begin(), taus.end());
  std::vector<Bond> bondsVec(bonds.begin(), bonds.end());

  std::cout << "Testing getNumBonds..." << std::endl;

  // Test getNumBonds
  c.addBonds(tausVec, bondsVec);
  assert(c.getNumBonds() == bondsVec.size());

  std::cout << std::endl;

}

void testEquality(std::array<double, 4> taus, std::array<Bond, 4> bonds) {
  Bond b({3, 4});
  Bond bb({1});
  Configuration c(1e-5); Configuration cc(1e-5);

  std::cout << "Testing equality operator..." << std::endl;

  // Test equality and inequality operators
  assert(c == cc);
  c.addBond(0.27, b);
  assert(c != cc);
  cc.addBond(0.30, b);
  assert(c != cc);
  c.addBond(0.30, b);
  assert(c != cc);
  c.delBond(0.27);
  assert(c == cc);
  cc.delBond(0.30);
  cc.addBond(0.30, bb);
  assert(c != cc);

  std::cout << std::endl;

}

void testTruncateToTolerance() {
  double tau = 2.71401;
  double tauDiff = tau + 2e-5;
  Bond b({1});
  Configuration c(1e-5);

  std::cout << "Testing truncateToTolerance..." << std::endl;

  // Test getting the same bond outside the given tolerance
  c.addBond(tau, b);
  Bond bb = c.getBond(tauDiff);
  assert(bb.getNumSites() == 0);

  // Test getting the same bond within the given tolerance
  tauDiff = tau + 1e-6;
  b = c.getBond(tau);
  bb = c.getBond(tauDiff);
  assert(b == bb);

  std::cout << std::endl;

}
