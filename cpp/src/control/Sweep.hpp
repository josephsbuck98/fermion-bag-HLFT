#pragma once

#include <map>
#include <vector>

#include "Configuration.hpp"
#include "Input.hpp"
#include "Lattice.hpp"
// #include "Update.hpp"

//TODO: Just pass in controlInput?
class Sweep { //This is the only class that will both define the loop structure AND handle the majority of the inner workings of each iteration. It will loop over the different time groupings, and within each grouping it will loop for a specified number of times, calling update each iteration.,
public:
  Sweep(int initNumTimeGroups, double scaleNumUpdates);

  void run(Configuration& configuration, const Lattice& lattice);

private: 
  int initNumTimeGroups;
  double scaleNumUpdates;
  // int numUpdatesPerGroup; //TODO: This will be different for every group, b/c Nsites is different for each 
  
  std::vector<int> finNumBondsPerGroup; //NOTE: The length of this vector will be equal to initNumTimeGroups, not finNumTimeGroups, b/c in order to report the number of bonds in each time group, you have to use the same time groups you just used.
  std::map<int, int> finNumBondsPerType;
  int finNumTimeGroups;
  int finNumAccepts;
  int finNumRejects;
  int finNumInserts;
  int finNumRemoves;
  int finNumBonds;

};