// Entry point for running simulations
#include <iostream>

#include "Configuration.hpp"
#include "Driver.hpp"
#include "InputParser.hpp"
#include "Lattice.hpp"
#include "Output.hpp"

int main(int argc, char* argv[]) {
  // Ensure you get an input file
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <input.yaml>\n";
    return 1;
  }


  // Read in the inputs and handle errors
  InputParser::ParsedInput input;
  std::string filename = argv[1];
  std::cout << "Reading input file..." << std::endl;
  try {
    input = InputParser::parseInputFile(filename);
  } catch (const std::exception& e) {
    std::cerr << "Error parsing input file: " << e.what() << "\n";
    return 1;
  }
  std::cout << "Input parameters successfully imported and validated.\n\n";


  // Generate initial Configuration, Lattice, and Hamiltonian classes
  std::cout << "Generating lattice and initial configuration.\n"; 
  Configuration configuration = Configuration(input.configurationInput);
  Lattice lattice = Lattice(input.latticeInput);
  std::cout << "Finished generating lattice and initial configuration.\n\n";


  // Initialize the output directory and load restart file (if user specified)
  Output output(input, filename);
  if (input.outputInput.restarts) {
    output.readRestartFile(configuration);
  }

  
  // Create Driver object and call its .run() function.
  Driver driver = Driver(input);
  driver.run(configuration, lattice); //TODO: Pass in Output class


  return 0;
}
