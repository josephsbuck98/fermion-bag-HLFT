#pragma once

#include <vector>

class Bond {
public:
  Bond(int numSites, const std::vector<int>& indices);

  int getNumSites() const;
  const std::vector<int>& getIndices() const;

private:
  int numSites;
  std::vector<int> indices;

};
