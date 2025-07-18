#include <cassert>
#include <iostream>

#include "Bond.hpp"
#include "Configuration.hpp"

void testAddBond(std::array<double, 4> taus, std::array<Bond, 4> bonds, Configuration& c);
void testDelBond(std::array<double, 4> taus, std::array<Bond, 4> bonds, Configuration& c);
void testGetBond(std::array<double, 4> taus, std::array<Bond, 4> bonds, Configuration& c);
void testGetBonds(std::array<double, 4> taus, std::array<Bond, 4> bonds, Configuration& c);
void testGetNumBonds(std::array<double, 4> taus, std::array<Bond, 4> bonds, Configuration& c);
void testEquality(std::array<double, 4> taus, Configuration& c, Configuration& d);

void runConfigurationTests() {
  Bond b1({1});
  Bond b2({2, 3});
  Bond b3({4, 5, 6});
  Bond b4({3, 4});
  std::array<double, 4> taus = {0.01, 0.025, 0.1, 0.45};
  std::array<Bond, 4> bonds = {b1, b2, b3, b4};
  Configuration c;
  Configuration d;

  std::cout << "Running configuration tests...\n";

  testAddBond(taus, bonds, c);
  testDelBond(taus, bonds, c);
  testGetBond(taus, bonds, c);

  c = Configuration();
  d = Configuration();
  for(int i = 0; i < taus.size(); i++) {
    c.addBond(taus[i], bonds[i]);
    d.addBond(taus[i], bonds[i]);
  }
  testGetBonds(taus, bonds, c);
  testGetNumBonds(taus, bonds, c);

  std::cout << "Completed configuration tests.";
}

void testAddBond(std::array<double, 4> taus, std::array<Bond, 4> bonds, Configuration& c) {
  for(int i = 0; i < taus.size(); i++) {
    c.addBond(taus[i], bonds[i]);
    assert(c.getNumBonds() == i + 1);
  }
}

void testDelBond(std::array<double, 4> taus, std::array<Bond, 4> bonds, Configuration& c) {
  //TODO: Test deleting a bond that isn't there
  for(int i = taus.size(); i > 0; i--) {
    c.delBond(taus[i]);
    assert(c.getNumBonds() == i - 1);
  }
}

void testGetBond(std::array<double, 4> taus, std::array<Bond, 4> bonds, Configuration& c) {
  //TODO: Test getting a bond that isn't there
  c.addBond(taus[0], bonds[0]);
  Bond b = c.getBond(taus[0]);
  for(int i = 0; i < b.getIndices().size(); i++) {
    assert(b.getIndices()[i] == bonds[0].getIndices()[i]);
  }
  
  try {
    Bond bb = c.getBond(taus[1]);
    assert(false);
  } catch (const std::exception& e) {
    assert(true);
  }
}

void testGetBonds(std::array<double, 4> taus, std::array<Bond, 4> bonds, Configuration& c) {
  std::map<double, Bond> cBonds = c.getBonds();
  for (int i = 0; i < taus.size(); i++) {
    assert(cBonds.at(taus[i]) == bonds[i]);
  }
}

void testGetNumBonds(std::array<double, 4> taus, std::array<Bond, 4> bonds, Configuration& c) {
  assert(c.getNumBonds() == bonds.size());
}

void testEquality(std::array<double, 4> taus, Configuration& c, Configuration& d) {
  Bond b({3, 4});
  Bond bb({1});

  assert(c == d);
  d.addBond(0.27, b);
  assert(c != d);
  d.delBond(0.27);
  assert(c == d);
  d.delBond(taus[0]);
  d.addBond(0.001, b);
  assert(c != d);
  d.delBond(taus[0]);
  d.addBond(taus[0], bb);
  assert(c != d);
}