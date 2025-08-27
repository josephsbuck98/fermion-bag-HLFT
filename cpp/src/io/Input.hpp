#pragma once

#include <yaml-cpp/yaml.h>
#include <iostream>
#include <map>
#include <numbers>
#include <string>
#include <vector>

#include "Constants.hpp"
#include "YAMLHelpers.hpp"

// The function templates in the YAML namespace tell YAML how to decode the
// input file into the correct variables of the correct classes.

//TODO: Unify all functions that do string input validation and use enums

//TODO: SWITCH ALL VARIABLES IN THE ACTUAL CODE TO CAMEL CASE

//TODO: Make the variable read-ins safe by checking that first of all a value
//TODO: exists associated with the current node, and that the value is the 
//TODO: type or class that you need. Use .IsScalar() for numeric fields.

//TODO: Have the program create a default ParsedInput obj with all the minimal
//TODO: default values. Then it gets overwritten and added to by actual input.

struct ControlInput {
  constants::HamilModel hamil_model;
  int nbonds_stop_sweeps = 3;
  double nbonds_stop_tol = 0.1;
  int max_sweeps = 100;
  double insert_prob = 0.5;

  void validate() const {
    if (nbonds_stop_sweeps <= 0) throw std::runtime_error("ControlInput: "
        "'nbonds_stop_sweeps' must be greater than 0.");
    if (nbonds_stop_tol < 0 || nbonds_stop_tol > 1) throw std::runtime_error(""
        "ControlInput: 'nbonds_stop_tol' must be between 0 and 1.");
    if (max_sweeps < 0) throw std::runtime_error("ControlInput: 'max_sweeps'"
        "must be greater than 0.");
    if (insert_prob < 0 || insert_prob > 1) throw std::runtime_error(""
        "ControlInput: 'insert_prob' must be between 0 and 1.");
  }
};

namespace YAML {
template<>
struct convert<ControlInput> {
  static bool decode(const Node& node, ControlInput& rhs) {
    if (!node.IsMap()) return false;

    if (node["hamil_model"]) {
      std::string hamil_type = getRequiredScalar<std::string>(node, "hamil_model");
      auto it_HA = constants::HAMIL_MODEL_MAP.find(hamil_type);
      if (it_HA == constants::HAMIL_MODEL_MAP.end()) {
        std::string message = "ControlInput: "
          "'hamil_model' must be one of: ";
        for (const auto& kv : constants::HAMIL_MODEL_MAP) {
          message += kv.first + ", ";
        }
        throw std::runtime_error(message.substr(0, message.size() - 2) + ".");
      }
      rhs.hamil_model = it_HA->second;
    }
    if (node["nbonds_stop_sweeps"]) rhs.nbonds_stop_sweeps = 
        getRequiredScalar<int>(node, "nbonds_stop_sweeps");
    if (node["nbonds_stop_tol"]) rhs.nbonds_stop_tol = 
        getRequiredScalar<double>(node, "nbonds_stop_tol");
    if (node["max_sweeps"]) rhs.max_sweeps = 
        getRequiredScalar<int>(node, "max_sweeps");
    if (node["insert_prob"]) rhs.insert_prob = 
        getRequiredScalar<double>(node, "insert_prob");

    return true;
  }
};
}



struct LatticeInput {
  constants::LatticeType type;

  double a = 0.0, b = 0.0, c = 0.0;
  double alpha = 0.0, beta = 0.0, gamma = 0.0;

  //TODO: Don't set defaults for other params, b/c might use their absence to infer dimensionality

  constants::BoundType x_bc_type, y_bc_type, z_bc_type;
  double x_min = 0.0, y_min = 0.0, z_min = 0.0;
  double x_nsites = 100, y_nsites = 100, z_nsites = 100;
  std::string x_base = "a", y_base = "b", z_base = "c";

  void validate() const {
    if (a <= 0) throw std::runtime_error("LatticeInput: 'a' must be positive"); //TODO: Validate bc_types and lattice_types
    if (x_nsites < 1) throw std::runtime_error("LatticeInput: 'x_max_fac' "
      "must be greater than 1.");
    if ((alpha > 0 && alpha > constants::pi) || (beta > 0 && beta > constants::pi)
      || (gamma > 0 && gamma > constants::pi)) throw std::runtime_error(
      "Lattice Input: angles must be between 0 and pi.");
  }
};

namespace YAML {
template<>
struct convert<LatticeInput> {
  static bool decode(const Node& node, LatticeInput& rhs) {
    if (!node.IsMap()) return false;

    if (node["type"]) {
      std::string lat_type = getRequiredScalar<std::string>(node, "type");
      auto it_LAT = constants::LATTICE_TYPE_MAP.find(lat_type);
      if (it_LAT == constants::LATTICE_TYPE_MAP.end()) {
        std::string message = "LatticeInput: 'type' must be one of: ";
        for (const auto& kv : constants::LATTICE_TYPE_MAP) {
          message += kv.first + ", ";
        }
        throw std::runtime_error(message.substr(0, message.size() - 2) + ".");
      }
      rhs.type = it_LAT->second;
    }
    
    if (node["a"]) rhs.a = getRequiredScalar<double>(node, "a");
    if (node["b"]) rhs.b = getRequiredScalar<double>(node, "b");
    if (node["c"]) rhs.c = getRequiredScalar<double>(node, "c");
    if (node["alpha"]) rhs.alpha = getRequiredScalar<double>(node, "alpha");
    if (node["beta"]) rhs.beta = getRequiredScalar<double>(node, "beta");
    if (node["gamma"]) rhs.gamma = getRequiredScalar<double>(node, "gamma");

    if (node["lims"]) {
      if (node["lims"]["x"]) {
        auto x = node["lims"]["x"];
        if (x["bc_type"]) {
          std::string x_bc = getRequiredScalar<std::string>(x, "bc_type");
          auto it_BND = constants::BOUND_TYPE_MAP.find(x_bc);
          if (it_BND == constants::BOUND_TYPE_MAP.end()) {
            std::string message = "LatticeInput: 'x_bc_type' must be one of: ";
            for (const auto& kv : constants::BOUND_TYPE_MAP) {
              message += kv.first + ", ";
            }
            throw std::runtime_error(message.substr(0, message.size() - 2) + ".");
          }
          rhs.x_bc_type = it_BND->second;
        }
        
        if (x["min"]) rhs.x_min = getRequiredScalar<double>(x, "min");
        if (x["nsites"]) rhs.x_nsites = getRequiredScalar<double>(x, "nsites");
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
  int num_time_groups_init = 5;
  double scale_updates_per_sweep = 1;
  std::map<int, double> bond_type_props; //TODO: Might fit better with the other hamiltonian parameters in control? May need to put those hamiltonian parameters in a "hamiltonian" section

  void validate() const { //TODO: Validate bond_type_props!!!!!
    if (float_tol < 1e-15) throw std::runtime_error("ConfigurationInput: "
        "'float_tol' must be greater than 1e-15 and non-negative.");
    if (beta <= 0) throw std::runtime_error("ConfigurationInput: "
        "'beta' must be greater than 0.");
    if (num_time_groups_init <= 0) throw std::runtime_error("ConfigurationInput: "
        "'num_time_groups_init' must be greater than 0."); 
    if (scale_updates_per_sweep <= 0) throw std::runtime_error("ConfigurationInput: "
        "'scale_updates_per_sweep' must be greater than 0.");
  } //NOTE: Don't validate bond_type_props now, because it will be replaced by physics in the future.
};

namespace YAML {
template<>
struct convert<ConfigurationInput> {
  static bool decode(const Node& node, ConfigurationInput& rhs) {
    if (!node.IsMap()) return false;
    if (node["float_tol"]) rhs.float_tol = getRequiredScalar<double>(node, "float_tol");
    if (node["beta"]) rhs.beta = getRequiredScalar<double>(node, "beta");
    if (node["num_time_groups_init"]) rhs.num_time_groups_init = 
        getRequiredScalar<int>(node, "num_time_groups_init");
    if (node["scale_updates_per_sweep"]) rhs.scale_updates_per_sweep =
        getRequiredScalar<double>(node, "scale_updates_per_sweep");
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