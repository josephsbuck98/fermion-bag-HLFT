// Entry point for running simulations
#include <iostream>

#include "Configuration.hpp"
#include "Driver.hpp"
#include "InputParser.hpp"
#include "Lattice.hpp"
#include "Output.hpp"
#include "RandomHelpers.hpp"

//TODO: SHOULD I BE PASSING ALL INPUT OBJECTS AS CONST & SO COPIES AREN'T BEING MADE EVERYWHERE?



int main(int argc, char* argv[]) {
  // Ensure you get an input file
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " \"input.yaml\" > \"output.txt\"\n";
    return 1;
  }


  // Read in the inputs and handle errors
  std::string filename = argv[1];
  std::cout << "Reading input file..." << std::endl;
  InputParser::ParsedInput input;
  try {
    input = InputParser::parseInputFile(filename);
  } catch (const std::exception& e) {
    std::cerr << "Error parsing input files: " << e.what() << "\n";
    return 1;
  }
  std::cout << "Input parameters successfully imported and validated.\n\n";



  // Initialize Configuration, Lattice, and Output classes
  std::cout << "Generating lattice and initial configuration.\n"; 
  int startSweepId = 0;
  Configuration configuration = Configuration(input.configurationInput);
  Lattice lattice = Lattice(input.latticeInput);
  Output output(input, filename, configuration);
  std::cout << "Finished initializing configuration, lattice, and output.\n\n";



  // Init restartInput, load RESTART if spec, overwrite userin if RESTART exists
  RestartInputParser::ParsedRestartInput restartInput;
  fs::path restartPath = output.getRestartPath();
  if (fs::exists(restartPath)) input.outputInput.restarts = true;
  if (input.outputInput.restarts && fs::exists(restartPath)) {
    std::cout << "An output file was present...\n\n\n" << std::endl;
    restartInput = RestartInputParser::parseRestartInputFile(restartPath);
  }



  // If restart was populated, use its data accordingly
  if (restartInput.restartPopulated) {
    //TODO: Note that on restart, it is possible that parameters such as write out frequency were changed, which will break current write out logic.
    startSweepId = restartInput.currSweepId;
    configuration = restartInput.configuration;
    restoreRNG(restartInput.seed, restartInput.state);
  } else {
    globalRNG(input.controlInput.randomSeed);
  }



  // Create Driver object and call its .run() function.
  Driver driver = Driver(input);

  driver.run(configuration, lattice, output, startSweepId);
  //TODO: USER SHOULD ALWAYS BE ABLE TO SPECIFY THE MAX SWEEPS, BUT THEY SHOULD ALSO BE ABLE TO SPECIFY THE NUMBER OF SWEEPS BEYOND THE END OF THE RESTART FILE THAT THEY WANT TO DO. MAX SWEEPS WILL ALWAYS OVERRIDE. 

  return 0;
}
