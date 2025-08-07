#include <yaml-cpp/yaml.h>
#include <stdexcept>
#include <iostream>

#include "InputParser.hpp"

using namespace InputParser;


namespace InputParser {


ParsedInput parseInputFile(const std::string& filepath) {
  ControlInput controlInput;
  LatticeInput latticeInput;
  ConfigurationInput configurationInput;
  
  ParsedInput parsedInput;
  parsedInput.controlInput = ControlInput();
  parsedInput.latticeInput = LatticeInput();
  parsedInput.configurationInput = ConfigurationInput();

  YAML::Node root = YAML::LoadFile(filepath);

  // Must create a ParsedInput object (ParsedInput is a struct)
  // Contains a control input object, lattice input object, and configuration input object 

  return parsedInput;
};

}
