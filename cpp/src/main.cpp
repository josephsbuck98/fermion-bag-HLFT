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
  std::cout << "Reading input file..." << std::endl;
  try {
    input = InputParser::parseInputFile(filepath);
  } catch (const std::exception& e) {
    std::cerr << "Error parsing input file: " << e.what() << "\n";
    return 1;
  }
  std::cout << "Input parameters successfully imported and validated.\n\n";


  // Perform cross validation between input classes


  // Report some key parameters or variables perhaps not given in input file.


  // Generate initial configuration (empty configuration) and Lattice.


  // Initiate insert/delete process.


  //TODO: Store number of each type of bond at each time step.

  // Return
  
  

  return 0;
}
