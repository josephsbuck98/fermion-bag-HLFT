#include <cassert>
#include <iostream>

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

    std::cout << ">>>>>Completed LatticeTests.\n\n";
};

void testLimsKeys() {

};

void testNptsKeys() {

};

void testValidBounds() {

};

void testValidNpts() {

};

void testFiveSiteIntegerLattice() {

};
