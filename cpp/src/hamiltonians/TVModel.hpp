#pragma once

#include "Configuration.hpp"
#include "Lattice.hpp"


class TVModel {
public:
  TVModel(InputParser::ParsedInput input);

  void applyUpdate(Configuration& configuration, const Lattice& lattice) const;

private:

};