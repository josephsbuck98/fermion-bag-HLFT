#include <yaml-cpp/yaml.h>
#include "InputParser.hpp"

namespace InputParser {

ParsedInput parseInputFile(const std::string& filepath) {
  ParsedInput pI = YAML::LoadFile(filepath).as<ParsedInput>();

  // Carry out any final changes
  pI.configurationInput.initNumTimeGroups = pI.controlInput.initNumTimeGroups;

  return pI;
};
}
