#pragma once

#include <vector>

// #include "Update.hpp"


class Sweep {
public:
  Sweep();

private: 
  //TODO: Move num_time_groups_init to controlInput
  //TODO: Create acceptProb input 
  int initNumTimeGroups;
  double scaleUpdates;
  // int numUpdatesPerGroup; //TODO: This will be different for every group, b/c Nsites is different for each 
  
  std::vector<int> finNumBondsPerGroup; //NOTE: The length of this vector will be equal to initNumTimeGroups, not finNumTimeGroups.
  int finNumTimeGroups;
  int finNumAccepts;
  int finNumRejects;
  int finNumInserts;
  int finNumRemoves;
  int finNumBonds;

};