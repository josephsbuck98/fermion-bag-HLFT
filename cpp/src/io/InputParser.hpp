#pragma once

#include <string>
#include <map>
#include "Input.hpp"

namespace InputParser {

  struct ParsedInput {
    ControlInput controlInput;
    LatticeInput latticeInput;
    ConfigurationInput configurationInput;

    void validate() const {
      controlInput.validate();
      latticeInput.validate();
      configurationInput.validate();

      if (latticeInput.type == constants::LatticeType::SIMPLE_CUBIC) {
        // Require a, x_min, x_nsites, x_bc_type. Already there, so no action required.
      }
      if (controlInput.hamil_model == constants::HamilModel::RANDOM) {
        // Require nbonds_stop_sweeps, nbonds_stop_tol, max_sweeps, insert_prob, num_time_groups_init, scale_updates_per_sweep
      }
    }//TODO: Validate y, z, honeycomb, and other hamiltonians later.
  };

  ParsedInput parseInputFile(const std::string& filepath);

}

namespace YAML {
template<>
struct convert<InputParser::ParsedInput> {
  static bool decode(const Node& node, InputParser::ParsedInput& rhs) {
    if (!node.IsMap()) return false;
    if (node["control"]) rhs.controlInput = node["control"].as<ControlInput>();
    if (node["lattice"]) rhs.latticeInput = node["lattice"].as<LatticeInput>();
    if (node["configuration"]) rhs.configurationInput = node["configuration"].as<ConfigurationInput>();
    
    rhs.validate();
    return true;
  }
};
}