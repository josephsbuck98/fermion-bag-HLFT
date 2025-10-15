#pragma once

#include "Configuration.hpp"
#include "InputParser.hpp"
#include "Lattice.hpp"
#include "Sweep.hpp"


class TVModel {
public:
  TVModel(InputParser::ParsedInput input);

  consts::BondActionType applyUpdate(Configuration& configuration, 
      const Lattice& lattice, int groupLowerBound, int groupUpperBound, 
      int groupNum, Configuration::RegionData regionData) const;

private:

};