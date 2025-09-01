#pragma once

#include <map>

#include "Configuration.hpp"
#include "InputParser.hpp"
#include "Lattice.hpp"


class Random {
public:
  Random(InputParser::ParsedInput input);
  void normalizeBondTypeProps();

  consts::BondActionType applyUpdate(Configuration& configuration, 
      const Lattice& lattice, int groupLowerBound, int groupUpperBound) const;

  void handleInsert(Configuration& configuration, const Lattice& lattice,
    int groupLowerBound, int groupUpperBound) const;

  void handleRemoval(Configuration& configuration, int groupLowerBound,
     int groupUpperBound) const;
  
private:
  double acceptProb = 0.5;
  double insertProb = 0.5;
  std::map<int, double> bondTypeProps = {
    {1, 0.10}, {2, 0.20}, {3, 0.30}, {4, 0.25}, {5, 0.15}
  }; //TODO: Validate values when importing. Any missing or 
};