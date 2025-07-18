#pragma once

#include <vector>

class Bond {
public:
  Bond(const std::vector<int>& indices);

  int getNumSites() const;
  const std::vector<int>& getIndices() const;

  bool operator==(const Bond& other) const;
  bool operator!=(const Bond& other) const;

private:
  int numSites;
  std::vector<int> indices;

};
