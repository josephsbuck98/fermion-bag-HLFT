#pragma once

#include <iostream>
#include <set>

#include "SimpleCubic.hpp"


class Site; // Forward declaration

class Bond { 
public:
  Bond(const std::set<int>& indices);
  Bond(const std::set<const Site*>& sites);

  int getNumSites() const;
  const std::set<int>& getIndices() const;

  bool operator==(const Bond& other) const;
  bool operator!=(const Bond& other) const;

  friend std::ostream& operator<<(std::ostream& os, const Bond& bond);

private:
  int numSites;
  std::set<int> indices; //TODO: Change to support multiple dims (use enums like in Lattice)

  std::set<const Site*> sites;
};
