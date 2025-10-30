#pragma once

#include <yaml-cpp/yaml.h>

#include <iostream>
#include <map>
#include <numbers>
#include <string>
#include <vector>

#include "Constants.hpp"
#include "EnumHelpers.hpp"
#include "YAMLHelpers.hpp"

// The function templates in the YAML namespace tell YAML how to decode the
// input file into the correct variables of the correct classes.

//TODO: SWITCH ALL VARIABLES IN THE ACTUAL CODE TO CAMEL CASE

struct ControlInput {
  int equilSweepsPatience = 3;
  double equilSweepsTol = 0.1;
  double scaleNumUpdates = 1;
  int maxSweeps = 100; 
  int numTimeGroups = 5;
  uint64_t randomSeed = 0;

  void validate() const {
    if (equilSweepsPatience <= 0) throw std::runtime_error("ControlInput: "
        "'equil_sweeps_patience' must be greater than 0.");
    if (equilSweepsTol < 0 || equilSweepsTol > 1) throw std::runtime_error(""
        "ControlInput: 'equil_sweeps_tol' must be between 0 and 1.");
    if (scaleNumUpdates <= 0) throw std::runtime_error("ConfigurationInput: "
        "'scale_num_updates' must be greater than 0.");
    if (maxSweeps < 0) throw std::runtime_error("ControlInput: 'max_sweeps'"
        "must be greater than 0.");
    if (numTimeGroups <= 0) throw std::runtime_error("ConfigurationInput: "
        "'num_time_groups' must be greater than 0.");
  }

  friend std::ostream& operator<<(std::ostream& os, 
      const ControlInput& contIn) {
    os << "  Control:\n";
    os << "    Maximum number of sweeps - " << contIn.maxSweeps << "\n";
    os << "    Equilibration tolerance & patience - " << 
        contIn.equilSweepsTol << " & " << contIn.equilSweepsPatience << "\n";
    os << "    Random seed - " << contIn.randomSeed << "\n";
    return os;
  }
};

namespace YAML {
template<>
struct convert<ControlInput> {
  static bool decode(const Node& node, ControlInput& rhs) {
    if (!node.IsMap()) return false;

    if (node["equil_sweeps_patience"]) rhs.equilSweepsPatience = 
        getRequiredScalar<int>(node, "equil_sweeps_patience");
    if (node["equil_sweeps_tol"]) rhs.equilSweepsTol = 
        getRequiredScalar<double>(node, "equil_sweeps_tol");
    if (node["scale_num_updates"]) rhs.scaleNumUpdates =
        getRequiredScalar<double>(node, "scale_num_updates");
    if (node["max_sweeps"]) rhs.maxSweeps = 
        getRequiredScalar<int>(node, "max_sweeps");
    if (node["num_time_groups"]) rhs.numTimeGroups = 
        getRequiredScalar<int>(node, "num_time_groups");
    if (node["random_seed"]) rhs.randomSeed = 
        getRequiredScalar<uint64_t>(node, "random_seed");
    
    return true;
  }
};
}



struct OutputInput {
  int outSweepsPatience = 20;
  std::string outDirName = "";
  bool writeBondsPerType = false;
  bool restarts = false;

  void validate() const {
    if (outSweepsPatience < 1) throw std::runtime_error("OutputInput: "
      "'out_sweeps_patience' must be greater than 1.");
    if (outDirName.empty() || outDirName.find("/") != std::string::npos) 
      throw std::runtime_error("OutputInput: 'out_dir' must be non-empty and "
      "must not contain the '/' character.");
  }

  friend std::ostream& operator<<(std::ostream& os, 
      const OutputInput& outIn) {
    os << "  Output:\n";
    os << "    Output directory - " << outIn.outDirName << "\n";
    os << "    Number of sweeps between outfile writes - " << 
        outIn.outSweepsPatience << "\n";
    if (outIn.writeBondsPerType) os << "    Will output bondsPerType\n";
    if (outIn.restarts) os << "    Will output RESTART files\n";
    return os;
  }
};

namespace YAML {
template <>
struct convert<OutputInput> {
  static bool decode(const Node& node, OutputInput& rhs) {
    if (!node.IsMap()) return false;

    if (node["out_sweeps_patience"]) rhs.outSweepsPatience = 
      getRequiredScalar<int>(node, "out_sweeps_patience");
    if (node["out_dir"]) rhs.outDirName = 
      getRequiredScalar<std::string>(node, "out_dir");
    if (node["write_bonds_per_type"]) rhs.writeBondsPerType =
      getRequiredScalar<bool>(node, "write_bonds_per_type");
    if (node["restarts"]) rhs.restarts = 
      getRequiredScalar<bool>(node, "restarts");
    
    return true;
  }
};
}



struct LatticeInput {
  consts::LatticeType type;
  consts::DimsType dims;

  double a = 0.0, b = 0.0, c = 0.0;
  double alpha = 0.0, beta = 0.0, gamma = 0.0;

  //TODO: Don't set defaults for other params, b/c might use their absence to infer dimensionality

  consts::BoundType x_bc_type, y_bc_type, z_bc_type;
  double x_min = 0.0, y_min = 0.0, z_min = 0.0;
  double x_nsites = 100, y_nsites = 100, z_nsites = 100;

  void validate() const {
    if (a <= 0) throw std::runtime_error("LatticeInput: 'a' must be positive");
    if (x_nsites < 1) throw std::runtime_error("LatticeInput: 'x_max_fac' "
      "must be greater than 1.");
    if ((alpha > 0 && alpha > consts::pi) || (beta > 0 && beta > consts::pi)
      || (gamma > 0 && gamma > consts::pi)) throw std::runtime_error(
      "Lattice Input: angles must be between 0 and pi.");
  }

  friend std::ostream& operator<<(std::ostream& os, 
      const LatticeInput& latIn) {
    os << "  Lattice:\n";
    os << "    Lattice type - " << 
        keyFromValue<std::string, consts::LatticeType>
        (consts::LATTICE_TYPE_MAP, latIn.type) << "\n";
    os << "    Number of dimensions - " << 
        keyFromValue<int, consts::DimsType>
        (consts::DIMS_TYPE_MAP, latIn.dims) << "\n";
    os << "    Boundary conditions - " << //TODO: Output for each dim
        keyFromValue<std::string, consts::BoundType>
        (consts::BOUND_TYPE_MAP, latIn.x_bc_type) << "\n";
    os << "    Total number of sites - " << latIn.x_nsites << "\n"; //TODO: Compute based on dims
    os << "    Site spacing - " << latIn.a << "\n"; //TODO: Do for each dim
    return os;
  }
};

namespace YAML {
template<>
struct convert<LatticeInput> {
  static bool decode(const Node& node, LatticeInput& rhs) {
    if (!node.IsMap()) return false;

    assignFromMap<std::string>(node, "type", consts::LATTICE_TYPE_MAP, rhs.type, "LatticeInput");
    assignFromMap<int>(node, "dims", consts::DIMS_TYPE_MAP, rhs.dims, "LatticeInput");
    
    if (node["a"]) rhs.a = getRequiredScalar<double>(node, "a");
    if (node["b"]) rhs.b = getRequiredScalar<double>(node, "b");
    if (node["c"]) rhs.c = getRequiredScalar<double>(node, "c");
    if (node["alpha"]) rhs.alpha = getRequiredScalar<double>(node, "alpha");
    if (node["beta"]) rhs.beta = getRequiredScalar<double>(node, "beta");
    if (node["gamma"]) rhs.gamma = getRequiredScalar<double>(node, "gamma");

    if (node["lims"]) {
      if (node["lims"]["x"]) {
        auto x = node["lims"]["x"];
        assignFromMap<std::string>(x, "bc_type", consts::BOUND_TYPE_MAP, rhs.x_bc_type, "LatticeInput");

        if (x["min"]) rhs.x_min = getRequiredScalar<double>(x, "min");
        if (x["nsites"]) rhs.x_nsites = getRequiredScalar<double>(x, "nsites");
      }
    }

    return true;
  }
};
}



struct HamiltonianInput {
  consts::HamilModel model;
  double acceptProb = 0.5;
  double insertProb = 0.5;
  std::map<int, double> bondTypeProps; 

  void validate() const {
    if (acceptProb < 0 || acceptProb > 1) throw std::runtime_error(""
        "ControlInput: 'accept_prob' must be between 0 and 1.");
    if (insertProb < 0 || insertProb > 1) throw std::runtime_error(""
        "ControlInput: 'insert_prob' must be between 0 and 1.");
    // Don't need to validate model or bondTypeProps here because they are validated on read in.
  }

  friend std::ostream& operator<<(std::ostream& os, 
      const HamiltonianInput& hamilIn) {
    os << "  Hamiltonian:\n";
    os << "    Hamiltonian type - " << 
        keyFromValue<std::string, consts::HamilModel>
        (consts::HAMIL_MODEL_MAP, hamilIn.model) << "\n";
    os << "    Probability of accepting a proposed update - " << 
        hamilIn.acceptProb << "\n"; //TODO: Adjust what is printed depending on the model
    os << "    Probability of a proposal being an insert - " << 
        hamilIn.insertProb << "\n";
    return os;
  }
};

namespace YAML {
template<>
struct convert<HamiltonianInput> {
  static bool decode(const Node& node, HamiltonianInput& rhs) {
    if (!node.IsMap()) return false;

    assignFromMap<std::string>(node, "model", consts::HAMIL_MODEL_MAP, rhs.model, "HamiltonianInput");

    if (node["accept_prob"]) rhs.acceptProb = 
        getRequiredScalar<double>(node, "accept_prob");
    if (node["insert_prob"]) rhs.insertProb = 
        getRequiredScalar<double>(node, "insert_prob");
    if (node["bond_type_props"]) { //TODO: Throw this in a function so it doesn't clutter.
      for (auto bond_size : node["bond_type_props"]) { 
        if (!bond_size.first.IsScalar()) continue;
        int key = bond_size.first.as<int>();
        if (!bond_size.second || bond_size.second.IsNull() ||
            !bond_size.second.IsScalar()) { 
          rhs.bondTypeProps[key] = 0.0;
        } else {
          rhs.bondTypeProps[key] = bond_size.second.as<double>();
          if (rhs.bondTypeProps[key] < 0) rhs.bondTypeProps[key] = 0.0;
        }
      }
    }

    return true;
  }
};
}



struct ConfigurationInput {
  double float_tol = 1e-5;
  double beta = 0.0;
  int numTimeGroups = 5; //NOTE: Keep this here. It doesn't get read in here but it gets copied over from ControlInput
  int avgNbondsPerGroup = 30;

  void validate() const { 
    if (float_tol < 1e-15) throw std::runtime_error("ConfigurationInput: "
        "'float_tol' must be greater than 1e-15 and non-negative.");
    if (beta <= 0) throw std::runtime_error("ConfigurationInput: "
        "'beta' must be greater than 0.");
    if (avgNbondsPerGroup <= 1) throw std::runtime_error("ConfigurationInput: " 
        "'avg_nbonds_per_group' must be greater than 1.");
  }

  friend std::ostream& operator<<(std::ostream& os, 
      const ConfigurationInput& configIn) {
    os << "  Configuration:\n";
    os << "    Float tolerance for tau - " << configIn.float_tol << "\n";
    os << "    Beta (inverse temperature or imaginary time) - " 
        << configIn.beta << "\n";
    return os;
  }
};

namespace YAML {
template<>
struct convert<ConfigurationInput> {
  static bool decode(const Node& node, ConfigurationInput& rhs) {
    if (!node.IsMap()) return false;

    if (node["float_tol"]) rhs.float_tol = getRequiredScalar<double>(node, "float_tol");
    if (node["beta"]) rhs.beta = getRequiredScalar<double>(node, "beta");
    if (node["avg_nbonds_per_group"]) rhs.avgNbondsPerGroup = 
        getRequiredScalar<int>(node, "avg_nbonds_per_group");

    return true;
  }
};
}