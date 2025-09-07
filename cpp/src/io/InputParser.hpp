#pragma once

#include <string>
#include <map>
#include "Input.hpp"

namespace InputParser {

  struct ParsedInput {
    ControlInput controlInput;
    OutputInput outputInput;
    LatticeInput latticeInput;
    HamiltonianInput hamiltonianInput;
    ConfigurationInput configurationInput;

    void validate() const {
      controlInput.validate();
      outputInput.validate();
      latticeInput.validate();
      hamiltonianInput.validate();
      configurationInput.validate();

      if (latticeInput.type == consts::LatticeType::SIMPLE_CUBIC) {
        if (latticeInput.dims == consts::DimsType::ONE) {

        }
        if (latticeInput.dims == consts::DimsType::TWO) {

        }
        if (latticeInput.dims == consts::DimsType::THREE) {

        }
        // Check if dims is 1, 2, or 3. Require x's for 1, x's & y's for 2, etc.
        // Require a, x_min, x_nsites, x_bc_type. Already there, so no action required.
      }
      if (hamiltonianInput.model == consts::HamilModel::RANDOM) {
        // Require nbonds_stop_sweeps, nbonds_stop_tol, max_sweeps, insert_prob, num_time_groups_init, scale_num_updates
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
    if (node["output"]) rhs.outputInput = node["output"].as<OutputInput>();
    if (node["lattice"]) rhs.latticeInput = node["lattice"].as<LatticeInput>();
    if (node["hamiltonian"]) rhs.hamiltonianInput = 
      node["hamiltonian"].as<HamiltonianInput>();
    if (node["configuration"]) rhs.configurationInput = 
      node["configuration"].as<ConfigurationInput>();
    
    rhs.validate();
    return true;
  }
};
}