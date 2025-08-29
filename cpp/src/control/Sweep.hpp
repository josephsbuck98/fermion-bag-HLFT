#pragma once

#include <map>
#include <vector>

// #include "Update.hpp"


class Sweep { //This is the only class that will both define the loop structure AND handle the majority of the inner workings of each iteration. It will loop over the different time groupings, and within each grouping it will loop for a specified number of times, calling update each iteration.,
public:
  Sweep(int initNumTimeGroups, double scaleNumUpdates);

  void run();

private: 
  int initNumTimeGroups;
  double scaleNumUpdates;
  // int numUpdatesPerGroup; //TODO: This will be different for every group, b/c Nsites is different for each 
  
  std::vector<int> finNumBondsPerGroup; //NOTE: The length of this vector will be equal to initNumTimeGroups, not finNumTimeGroups.
  std::map<int, int> finNumBondsPerType;
  int finNumTimeGroups;
  int finNumAccepts;
  int finNumRejects;
  int finNumInserts;
  int finNumRemoves;
  int finNumBonds;

};