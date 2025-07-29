#pragma once

#include <iostream>
#include <map>
#include <vector>

class Lattice {
public:
  Lattice(std::map<std::string, std::pair<float, float>> lims, std::map<std::string, int> npts);
 
  int getNumSites();
  
  float operator[](const int index) const; 

private:
  int numSites;
  std::vector<float> sites; //TODO: Change to support multiple dims
};
