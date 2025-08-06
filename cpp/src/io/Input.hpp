#pragma once

#include <map>
#include <string>
#include <vector>

struct ControlInput {
  std::string algorithm = "random";
  int nmoves = 0;

};

struct LatticeInput {
  std::string type = "simple-cubic";

  double a = 0.0, b = 0.0, c = 0.0;
  double alpha = 0.0, beta = 0.0, gamma = 0.0;

  double x_min = 0.0, y_min = 0.0, z_min = 0.0;
  double x_max_factor = -1, y_max_factor = -1, z_max_factor = -1;
  std::string x_base = "a", y_base = "b", z_base = "c";

};


struct ConfigurationInput {
  double float_tol = 1e-5;
  int nbonds = 10;
  double tau_max = 0.0;
  std::map<int, double> bond_type_props;

};
