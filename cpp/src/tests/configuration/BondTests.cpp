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
  
  std::cout << ">>>>>Completed BondTests.\n\n\n";
};

void testGetNumSites() {
  Bond b({2, 3, 4, 5});

  std::cout << "Testing getNumSites...\n";

  assert(b.getNumSites() == 4);

  std::cout << std::endl;

};

void testGetIndices() {
  std::set<int> indices({2, 3, 4, 5}); 
  Bond b(indices);

  std::cout << "Testing getIndices...\n";

  assert(b.getIndices() == indices);

  std::cout << std::endl;

};

void testEquality() {
  Bond b({1, 2, 3});
  Bond bb({3, 1, 2});
  Bond bbb({1, 2});

  std::cout << "Testing equality operator...\n";

  assert(b == bb);
  assert(b != bbb);
  assert(bb != bbb);

  std::cout << std::endl;

}
