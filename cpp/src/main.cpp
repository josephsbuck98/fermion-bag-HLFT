// Entry point for running simulations
#include <iostream>

#include "Configuration.hpp"
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


  // Generate initial configuration (empty configuration) and Lattice.
  // Configuration curr_configuration 
  //     = Configuration(input.configurationInput.float_tol);

  Configuration curr_configuration = Configuration(input.configurationInput);

  Lattice lattice = Lattice(input.latticeInput);
  std::cout << "Number of Sites: " << lattice.getNumSites(consts::DirsType::X) << std::endl;
  std::cout << "Site at Index 10: " << lattice.getSite(consts::DirsType::X, 10) << std::endl;
  
  // Report some key parameters or variables perhaps not given in input file.
  //TODO: Report all non-trivial parameters of the code (but don't be pedantic).
  //TODO: REPORT COMPUTED LATTICE PARAMETERS HERE


  // Initiate insert/delete process.


  //TODO: Store number of each type of bond at each time step.

  // Return
  
  

  return 0;
}
