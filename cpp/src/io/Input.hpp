#pragma once

#include <yaml-cpp/yaml.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "YAMLHelpers.hpp"

// The function templates in the YAML namespace tell YAML how to decode the
// input file into the correct variables of the correct classes.

//TODO: SWITCH ALL VARIABLES IN THE ACTUAL CODE TO CAMEL CASE

//TODO: Make the variable read-ins safe by checking that first of all a value
//TODO: exists associated with the current node, and that the value is the 
//TODO: type or class that you need. Use .IsScalar() for numeric fields.

//TODO: Have the program create a default ParsedInput obj with all the minimal
//TODO: default values. Then it gets overwritten and added to by actual input.

struct ControlInput {
  std::string hamil_model = "random";
  int nmoves = 0;
  double insert_prob = 0.5;
};

namespace YAML {
template<>
struct convert<ControlInput> {
  static bool decode(const Node& node, ControlInput& rhs) {
    if (!node.IsMap()) return false;
    if (node["hamil_model"]) rhs.hamil_model = getRequiredScalar<std::string>(node, "hamil_model");
    if (node["nmoves"]) rhs.nmoves = getRequiredScalar<int>(node, "nmoves");
    if (node["insert_prob"]) rhs.insert_prob = getRequiredScalar<double>(node, "insert_prob");
    return true;
  }
};
}



struct LatticeInput {
  std::string type = "simple-cubic";

  double a = 0.0, b = 0.0, c = 0.0;
  double alpha = 0.0, beta = 0.0, gamma = 0.0;

  //TODO: Don't set defaults for other params, b/c might use their absence to infer dimensionality

  std::string x_bc_type = "open", y_bc_type = "open", z_bc_type = "open";
  double x_min = 0.0, y_min = 0.0, z_min = 0.0;
  double x_max_factor = -1, y_max_factor = -1, z_max_factor = -1;
  std::string x_base = "a", y_base = "b", z_base = "c";

};

namespace YAML {
template<>
struct convert<LatticeInput> {
  static bool decode(const Node& node, LatticeInput& rhs) {
    if (!node.IsMap()) return false;

    if (node["type"]) rhs.type = getRequiredScalar<std::string>(node, "type");
    if (node["a"]) rhs.a = getRequiredScalar<double>(node, "a");
    if (node["b"]) rhs.b = getRequiredScalar<double>(node, "b");
    if (node["c"]) rhs.c = getRequiredScalar<double>(node, "c");
    if (node["alpha"]) rhs.alpha = getRequiredScalar<double>(node, "alpha");
    if (node["beta"]) rhs.beta = getRequiredScalar<double>(node, "beta");
    if (node["gamma"]) rhs.gamma = getRequiredScalar<double>(node, "gamma");

    if (node["lims"]) {
      if (node["lims"]["x"]) {
        auto x = node["lims"]["x"];
        if (x["min"]) rhs.x_min = getRequiredScalar<double>(x, "min");
        if (x["max_factor"]) rhs.x_max_factor = getRequiredScalar<double>(x, "max_factor");
        if (x["base"]) rhs.x_base = getRequiredScalar<std::string>(x, "base");
      }
    }
    return true;
  }
};
}



struct ConfigurationInput {
  double float_tol = 1e-5;
  double beta = 0.0;
  std::map<int, double> bond_type_props; //TODO: Might fit better with the other hamiltonian parameters in control? May need to put those hamiltonian parameters in a "hamiltonian" section

};

namespace YAML {
template<>
struct convert<ConfigurationInput> {
  static bool decode(const Node& node, ConfigurationInput& rhs) {
    if (!node.IsMap()) return false;
    if (node["float_tol"]) rhs.float_tol = getRequiredScalar<double>(node, "float_tol");
    if (node["beta"]) rhs.beta = getRequiredScalar<double>(node, "beta");
    if (node["bond_type_props"]) {
      for (auto bond_size : node["bond_type_props"]) { //TODO: Handle empty bond_type_props
        if (bond_size.second.IsNull()) { //TODO: Don't handle validations like negatives here. Diff file for that.
          rhs.bond_type_props[bond_size.first.as<int>()] = 0.0;
        } else {
          rhs.bond_type_props[bond_size.first.as<int>()] = bond_size.second.as<double>();
        }
      }
    }
    return true;
  }
};
}