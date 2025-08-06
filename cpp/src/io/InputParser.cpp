#include <yaml-cpp/yaml.h>
#include <stdexcept>
#include <iostream>

#include "InputParser.hpp"

using namespace InputParser;


namespace {

}

ParsedInput parseInputFile(const std::string& filepath) {
  YAML::Node root = YAML::LoadFile(filepath);
  
};