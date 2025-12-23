#pragma once
#include "LatticeBase.hpp"
#include <memory>

inline LatticeBase* latticePtr = nullptr;

// inline LatticeBase& getLattice() {
//     return *latticePtr; 
// }

inline const LatticeBase& getConstLattice() {
    return *latticePtr;
}