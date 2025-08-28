#include <yaml-cpp/yaml.h>
#include "InputParser.hpp"

namespace InputParser {

ParsedInput parseInputFile(const std::string& filepath) {
  return YAML::LoadFile(filepath).as<ParsedInput>();
};

}
