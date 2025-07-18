#include <cassert>
#include <iostream>

#include "Bond.hpp"

void testGetNumSites();
void testGetIndices();
void testEquality();

void runBondTests() {
  std::cout << ">>>>>Running BondTests...\n";
  
  testGetNumSites();
  testGetIndices();
  testEquality();
  
  std::cout << ">>>>>Completed BondTests.\n\n";
};

void testGetNumSites() {
  Bond b({2, 3, 4, 5});
  assert(b.getNumSites() == 4);
};

void testGetIndices() {
  std::set<int> indices({2, 3, 4, 5}); 
  Bond b(indices);
  assert(b.getIndices() == indices);
};

void testEquality() {
  Bond b({1, 2, 3});
  Bond bb({3, 1, 2});
  Bond bbb({1, 2});

  assert(b == bb);
  assert(b != bbb);
  assert(bb != bbb);

}
