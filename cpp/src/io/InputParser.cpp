#include <yaml-cpp/yaml.h>
#include "InputParser.hpp"

//TODO: Put input validations in a separate file in the io directory.

namespace InputParser {

ParsedInput parseInputFile(const std::string& filepath) {
  return YAML::LoadFile(filepath).as<ParsedInput>();
};

}
