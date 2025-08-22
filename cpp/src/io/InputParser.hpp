#pragma once

#include <string>
#include <map>
#include "Input.hpp"

namespace InputParser {

  struct ParsedInput {
    ControlInput controlInput;
    LatticeInput latticeInput;
    ConfigurationInput configurationInput;
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
    return true;
  }
};
}