#include <cassert>
#include <iostream>
#include <string>

#include "InputParser.hpp"

void runInputTests(const std::string& filepath) {
  std::cout << ">>>>>Running input tests...\n";

  InputParser::ParsedInput parsedInput;

  parsedInput = InputParser::parseInputFile(filepath);

  std::cout << ">>>Completed input tests.\n\n\n";
}
