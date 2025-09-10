#include <yaml-cpp/yaml.h>
#include "InputParser.hpp"

namespace InputParser {

ParsedInput parseInputFile(const std::string& filename) {
  ParsedInput pI = YAML::LoadFile(filename).as<ParsedInput>();

  // Carry out any final changes
  pI.controlInput.initNumTimeGroups = pI.configurationInput.initNumTimeGroups;

  return pI;
};
}
