// Entry point for running simulations
#include <iostream>

#include "InputParser.hpp"

int main(int argc, char* argv[]) {
  // Ensure you get an input file
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <input.yaml>\n";
    return 1;
  }

  // Read in the inputs and handle errors
  InputParser::ParsedInput input;
  std::string filepath = argv[1];
  try {
    input = InputParser::parseInputFile(filepath);
  } catch (const std::exception& e) {
    std::cerr << "Error parsing input file: " << e.what() << "\n";
    return 1;
  }

  std::cout << "Algorithm: " << input.controlInput.algorithm << "\n";
  std::cout << "Nmoves: "    << input.controlInput.nmoves << "\n";

  std::cout << "Lattice type: " << input.latticeInput.type << "\n";
  std::cout << "a = "           << input.latticeInput.a << "\n";
  std::cout << "alpha = "       << input.latticeInput.alpha << "\n";

  for (auto& bond_size : input.configurationInput.bond_type_props) {
      std::cout << "Bond Proportions: " << bond_size.first << " -> " << bond_size.second << "\n";
  }

  return 0;
}
