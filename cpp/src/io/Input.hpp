#pragma once

#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <iostream>
#include <map>
#include <numbers>
#include <string>
#include <vector>

#include "Constants.hpp"
#include "EnumHelpers.hpp"
#include "YAMLHelpers.hpp"

namespace fs = std::filesystem;

// The function templates in the YAML namespace tell YAML how to decode the
// input file into the correct variables of the correct classes.

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
    os << "    Output directory - " << fs::current_path().string() << 
        "/" << outIn.outDirName << "\n";
    os << "    Number of sweeps between outfile writes - " << 
        outIn.outSweepsPatience << "\n";
    if (outIn.writeBondsPerType) os << "    Will output bondsPerType.dat\n";
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

  consts::BoundType xBCType, yBCType, zBCType;
  double xMin = 0.0, yMin = 0.0, zMin = 0.0;
  int xNSites = 0, yNSites = 0, zNSites = 0;

  void validateSC1D(double latConst, int nSites) const {
    if (latConst <= 0) throw std::runtime_error("LatticeInput: Lattice "
        "constant must be positive");
    if (nSites < 1) throw std::runtime_error("LatticeInput: Number of sites "
      "must be greater than 0 in active dimensions.");
  }

  void validate() const {
    if (a <= 0) throw std::runtime_error("LatticeInput: 'a' must be positive");
    if (xNSites < 1) throw std::runtime_error("LatticeInput: 'x_max_fac' "
      "must be greater than 1.");
    if ((alpha > 0 && alpha > consts::pi) || (beta > 0 && beta > consts::pi)
      || (gamma > 0 && gamma > consts::pi)) throw std::runtime_error(
      "Lattice Input: angles must be between 0 and pi.");
    
    //TODO: Enforce a constraint on total number of sites in order to keep matrix size low?
    if (type == consts::LatticeType::SIMPLE_CUBIC) {
      validateSC1D(a, xNSites); // ONE, TWO, THREE
      if (dims != consts::DimsType::ONE) {
        validateSC1D(b, yNSites); // TWO, THREE
        bool invalidAngle = (alpha <= 0 || alpha >= consts::pi);
        if (dims != consts::DimsType::TWO) {
          validateSC1D(c, zNSites); // THREE
          invalidAngle = (invalidAngle || (beta <= 0 || beta >= consts::pi ||
              gamma <= 0 || gamma >= consts::pi));
        }
        if (invalidAngle) {
          throw std::runtime_error("LatticeInput: Lattice angles must be "
              "greater than 0 radians and less than pi radians.");
        }
      }
    } else if (type == consts::LatticeType::HONEYCOMB) {
      if (dims != consts::DimsType::TWO) {
        throw std::runtime_error("LatticeInput: The Honeycomb lattice must "
            "be run in two dimensions.");
      }
    }
  }

  friend std::ostream& operator<<(std::ostream& os, 
      const LatticeInput& latIn) {
    os << "  Lattice:\n";
    os << "    Lattice type - " << 
        keyFromValue<std::string, consts::LatticeType>
        (consts::LATTICE_TYPE_MAP, latIn.type) << "\n";
    if (latIn.type == consts::LatticeType::SIMPLE_CUBIC) {
      os << "    Number of dimensions - " << 
          keyFromValue<int, consts::DimsType>
          (consts::DIMS_TYPE_MAP, latIn.dims) << "\n";
      os << "        Boundary conditions - Number of sites - Site spacing\n"; 
      os << "    x    " << keyFromValue<std::string, consts::BoundType>
          (consts::BOUND_TYPE_MAP, latIn.xBCType) << "    -    " << 
          latIn.xNSites << "    -    " << latIn.a << std::endl;
      if (latIn.dims != consts::DimsType::ONE) {
        os << "    y    " << keyFromValue<std::string, consts::BoundType>
            (consts::BOUND_TYPE_MAP, latIn.yBCType) << "    -    " <<
            latIn.yNSites << "    -    " << latIn.b << std::endl;
        if (latIn.dims != consts::DimsType::TWO) {
          os << "    z    " << keyFromValue<std::string, consts::BoundType>
            (consts::BOUND_TYPE_MAP, latIn.zBCType) << "    -    " <<
            latIn.zNSites << "    -    " << latIn.c << std::endl;
        }
      }
      int totNSites = latIn.xNSites;
      if (latIn.dims == consts::DimsType::TWO) totNSites *= latIn.yNSites;
      if (latIn.dims == consts::DimsType::THREE) totNSites *= latIn.zNSites;
      os << "    Total number of sites - " << totNSites << "\n";
    } else {

    }
   
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
      if (node["lims"]["x"]) { //TODO: Consolidate these next 3 if statements into 1 if possible
        auto x = node["lims"]["x"];
        assignFromMap<std::string>(x, "bc_type", consts::BOUND_TYPE_MAP, rhs.xBCType, "LatticeInput");
        if (x["min"]) rhs.xMin = getRequiredScalar<double>(x, "min");
        if (x["nsites"]) rhs.xNSites = getRequiredScalar<int>(x, "nsites");
      }
      if (node["lims"]["y"]) {
        auto y = node["lims"]["y"];
        assignFromMap<std::string>(y, "bc_type", consts::BOUND_TYPE_MAP, rhs.yBCType, "LatticeInput");
        if (y["min"]) rhs.yMin = getRequiredScalar<double>(y, "min");
        if (y["nsites"]) rhs.yNSites = getRequiredScalar<int>(y, "nsites");
      }
      if (node["lims"]["z"]) {
        auto z = node["lims"]["z"];
        assignFromMap<std::string>(z, "bc_type", consts::BOUND_TYPE_MAP, rhs.zBCType, "LatticeInput");
        if (z["min"]) rhs.zMin = getRequiredScalar<double>(z, "min");
        if (z["nsites"]) rhs.zNSites = getRequiredScalar<int>(z, "nsites");
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
  double t = 0.0;
  double V = 0.0;

  void validate() const {
    if (acceptProb < 0 || acceptProb > 1) throw std::runtime_error(""
        "HamiltonianInput: 'accept_prob' must be between 0 and 1.");
    if (insertProb < 0 || insertProb > 1) throw std::runtime_error(""
        "HamiltonianInput: 'insert_prob' must be between 0 and 1.");
    // Don't need to validate model or bondTypeProps here because they are validated on read in.
  }

  friend std::ostream& operator<<(std::ostream& os, 
      const HamiltonianInput& hamilIn) {
    os << "  Hamiltonian:\n";
    os << "    Hamiltonian Model - " << 
        keyFromValue<std::string, consts::HamilModel>
        (consts::HAMIL_MODEL_MAP, hamilIn.model) << "\n";
    os << "    Probability of accepting a proposed update - " << 
        hamilIn.acceptProb << "\n"; //TODO: Adjust what is printed depending on the model
    os << "    Probability of a proposal being an insert - " << 
        hamilIn.insertProb << "\n";
    if (hamilIn.model == consts::HamilModel::TVModel) {
      os << "    t: " << hamilIn.t << "\n";
      os << "    V: " << hamilIn.V << "\n";
    }
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
    if (node["t"]) {
      rhs.t = getRequiredScalar<double>(node, "t");
      if (std::abs(rhs.t) < 0.00000001) {
        rhs.t = 1;
        std::cout << "HamiltonianInput: User supplied t value was too close ";
        std::cout << "to 0. Defaulting to t=1." << std::endl;
      }
    }
    if (node["V"]) rhs.V = getRequiredScalar<double>(node, "V");

    return true;
  }
};
}



struct ConfigurationInput {
  double floatTol = 1e-5;
  double beta = 0.0;
  int numTimeGroups = 5; //NOTE: Keep this here. It doesn't get read in here but it gets copied over from ControlInput

  void validate() const { 
    if (floatTol < 1e-15) throw std::runtime_error("ConfigurationInput: "
        "'float_tol' must be greater than 1e-15 and non-negative.");
    if (beta <= 0) throw std::runtime_error("ConfigurationInput: "
        "'beta' must be greater than 0.");
  }

  friend std::ostream& operator<<(std::ostream& os, 
      const ConfigurationInput& configIn) {
    os << "  Configuration:\n";
    os << "    Float tolerance for tau - " << configIn.floatTol << "\n";
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

    if (node["float_tol"]) rhs.floatTol = getRequiredScalar<double>(node, "float_tol");
    if (node["beta"]) rhs.beta = getRequiredScalar<double>(node, "beta");

    return true;
  }
};
}