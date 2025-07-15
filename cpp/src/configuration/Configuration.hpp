#pragma once

#include <map>

#include "Bond.hpp"

class Configuration {
public:
  Configuration();

  // void initialize();
  void addBond(double tau, Bond newBond);
  void delBond(double tau);

  int getNumBonds();
  

private:
  std::map<double, Bond> configuration;
};