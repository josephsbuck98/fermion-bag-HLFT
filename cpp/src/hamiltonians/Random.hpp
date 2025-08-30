#pragma once

#include <vector>


class Random {
public:
  Random();

  void applyUpdate();

private:
  double acceptProb = 0.5;
  double insertProb = 0.5;
  std::vector<double> bondTypeProps = {0.10, 0.20, 0.30, 0.25, 0.15}; //TODO: Validate values when importing. Any missing or 
};