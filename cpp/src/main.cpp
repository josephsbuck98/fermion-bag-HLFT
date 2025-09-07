// Entry point for running simulations
#include <iostream>

#include "Configuration.hpp"
#include "Driver.hpp"
#include "Random.hpp"
#include "InputParser.hpp"
#include "Lattice.hpp"

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


  // Initialize the output directory
  //TODO: Check for existence. 
  //TODO: Create if nonexistant
  //TODO: Clear if exists


  // Generate initial Configuration, Lattice, and Hamiltonian classes
  std::cout << "Generating lattice and initial configuration.\n"; 
  Configuration configuration = Configuration(input.configurationInput);
  Lattice lattice = Lattice(input.latticeInput);
  std::cout << "Finished generating lattice and initial configuration.\n\n";

  
  // Create Driver object and call its .run() function.
  Driver driver = Driver(input);
  driver.run(configuration, lattice);

  
  // Report some key parameters or variables perhaps not given in input file.
  //TODO: Report all non-trivial parameters of the code (but don't be pedantic).
  //TODO: REPORT COMPUTED LATTICE PARAMETERS HERE


  // Call loop driver and pass in configuration and lattice by reference


  return 0;
}
