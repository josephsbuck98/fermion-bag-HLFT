#include <cassert>
#include <iostream>

#include "Bond.hpp"
#include "Configuration.hpp"

//TODO: Output messages for each test

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
  // testDelBond(taus, bonds, c);

  // c = Configuration(1e-5);
  // d = Configuration(1e-5);
  // for(int i = 0; i < taus.size(); i++) {
  //   c.addBond(taus[i], bonds[i]);
  //   d.addBond(taus[i], bonds[i]);
  // }
  // testGetBonds(taus, bonds, c);
  // testGetNumBonds(taus, bonds, c);

  std::cout << ">>>>>Completed configuration tests.\n\n";
}

void testAddBond(std::array<double, 4> taus, std::array<Bond, 4> bonds) {
  Configuration c(1e-5);
  for(int i = 0; i < taus.size(); i++) {
    c.addBond(taus[i], bonds[i]);
    assert(c.getNumBonds() == i + 1);
  }
}

void testAddBonds(std::array<double, 4> taus, std::array<Bond, 4> bonds) {
  Configuration c(1e-5); Configuration cc(1e-5);
  std::vector<double> tausVec(taus.begin(), taus.end());
  std::vector<Bond> bondsVec(bonds.begin(), bonds.end());
  c.addBonds(tausVec, bondsVec);
  for (int i = 0; i < taus.size(); i++) {
    cc.addBond(taus[i], bonds[i]);
  }
  assert(c == cc);
}

void testDelBond(std::array<double, 4> taus, std::array<Bond, 4> bonds) {
  //TODO: Test deleting a bond that isn't there. 
  Configuration c(1e-5);
  std::vector<double> tausVec(taus.begin(), taus.end());
  std::vector<Bond> bondsVec(bonds.begin(), bonds.end());
  c.addBonds(tausVec, bondsVec);
  for(int i = taus.size(); i > 0; i--) {
    c.delBond(taus[i]);
    std::cout << c.getNumBonds() << ", " << i - 1 << std::endl;
    assert(c.getNumBonds() == i - 1);
  }
}

void testGetBond(std::array<double, 4> taus, std::array<Bond, 4> bonds) {
  //TODO: Test getting a bond that isn't there
  Configuration c(1e-5);
  c.addBond(taus[0], bonds[0]);
  Bond b = c.getBond(taus[0]);
  assert(b == bonds[0]);
}

void testGetBonds(std::array<double, 4> taus, std::array<Bond, 4> bonds) {
  Configuration c(1e-5);
  std::map<double, Bond> cBonds = c.getBonds();
  for (int i = 0; i < taus.size(); i++) {
    assert(cBonds.at(taus[i]) == bonds[i]);
  }
}

void testGetNumBonds(std::array<double, 4> taus, std::array<Bond, 4> bonds) {
  Configuration c(1e-5);
  assert(c.getNumBonds() == bonds.size());
}

void testEquality(std::array<double, 4> taus, std::array<Bond, 4> bonds) {
  Bond b({3, 4});
  Bond bb({1});
  Configuration c(1e-5); Configuration cc(1e-5);

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
  double tau = 2.714001;
  double tauDiff = 2.714002;
  Bond b({1});
  Configuration c(1e-5);
  c.addBond(tau, b);

  std::cout << c.getBond(tau);
  std::cout << c.getBond(tauDiff);

  assert(c.getBond(tau) == c.getBond(tauDiff));

}