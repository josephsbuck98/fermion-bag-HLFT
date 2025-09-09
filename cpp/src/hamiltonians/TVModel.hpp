#pragma once

#include "Configuration.hpp"
#include "InputParser.hpp"
#include "Lattice.hpp"


class TVModel {
public:
  TVModel(InputParser::ParsedInput input);

  consts::BondActionType applyUpdate(Configuration& configuration, 
      const Lattice& lattice, int groupLowerBound, int groupUpperBound) const;

private:

};