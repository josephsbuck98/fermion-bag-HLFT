#pragma once

#include "Constants.hpp"


class Update {
public:
  Update();

  void run();

private:
  // consts::HamilModel hamilModel;
  // double acceptProb;
  // double insertProb;
  double groupLowerBound;
  double groupUpperBound;

};