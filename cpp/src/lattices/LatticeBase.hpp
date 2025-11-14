#pragma once

#include "Constants.hpp"


class LatticeBase {
public:
  virtual ~LatticeBase() = default;

  virtual void printInfo() const = 0;

  virtual int getNumSites(consts::DirsType dir) const = 0;

protected:
  consts::LatticeType type;

};