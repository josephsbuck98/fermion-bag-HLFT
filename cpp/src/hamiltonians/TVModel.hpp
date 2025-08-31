#pragma once

#include "Configuration.hpp"
#include "Lattice.hpp"


class TVModel {
public:
  TVModel();

  void applyUpdate(Configuration& configuration, const Lattice& lattice) const;

private:

};