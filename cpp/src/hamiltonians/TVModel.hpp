#pragma once

#include "Configuration.hpp"
#include "InputParser.hpp"
#include "LatticeBase.hpp"


class TVModel {
public:
  TVModel(InputParser::ParsedInput input);

  consts::BondActionType applyUpdate(Configuration& configuration, 
      int groupNum, Configuration::RegionData regionData, 
      const LatticeBase* lattice) const;

private:

};