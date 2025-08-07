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
