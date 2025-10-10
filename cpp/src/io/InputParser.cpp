#include <sstream>
#include <yaml-cpp/yaml.h>

#include "InputParser.hpp"



namespace InputParser {
ParsedInput parseInputFile(const std::string& filename) {
  ParsedInput pI = YAML::LoadFile(filename).as<ParsedInput>();

  // Carry out any final changes
  pI.controlInput.initNumTimeGroups = pI.configurationInput.initNumTimeGroups;

  return pI;
}
}



namespace RestartInputParser {
ParsedRestartInput parseRestartInputFile(const fs::path& restartPath) {
  ParsedRestartInput pRI;

  std::ifstream file(restartPath);
  std::string line;
  while (std::getline(file, line)) {
    size_t start = line.find("[["); size_t end = line.find("]]", start + 2);
    if (start == std::string::npos || end == std::string::npos) {
      std::cout << "NOT A TITLE..." << std::endl;
      std::cout << line << std::endl;
      continue;
    }
    std::string title = line.substr(start + 2, end - (start + 2));
    if (title == "CURR_SWEEP_ID") {
      if (std::getline(file, line)) {
        pRI.currSweepId = std::stoi(line) + 1;
        std::cout << "OBTAINED CURRSWEEPID: " << pRI.currSweepId << std::endl;
      }
    } else if (title == "RANDOM_SEED") {
      if (std::getline(file, line)) {
        pRI.seed = std::stoi(line);
        std::cout << "OBTAINED SEED: " << pRI.seed << std::endl;
      }
    } else if (title == "RNG_STATE") {
      if (std::getline(file, line)) {
        std::istringstream iss(line);
        iss >> pRI.state;
        std::cout << "OBTAINED STATE: " << pRI.state << std::endl;
      }
    } else if (title == "CONFIGURATION") { //Configuration must be last due to constraints with ifstream input reading
      if (file >> pRI.configuration) {
        std::cout << "SUCCESSFULLY READ IN CONFIGURATION" << std::endl;
      } else {
        std::cout << "Error reading in configuration." << std::endl;
        throw std::runtime_error("Error reading in configuration.");
      }
    } else {
      //TODO: Case where the title is garbage by some mistake. Throw error.
    }

    std::cout << "Section Title: " << title << std::endl;

  }

  std::cout << "Current Sweep ID: " << pRI.currSweepId << std::endl;
  std::cout << "Number of Bonds: " << pRI.configuration.getNumBonds() << std::endl;
  std::cout << "Random Seed: " << pRI.seed << std::endl;
  std::cout << "Random State: " << pRI.state << std::endl;

  file.close();





  //   pRI.restartPopulated = false;
  //   std::cout << "WARNING: RESTART file corrupted or incomplete. Disregard "
  //       "this message if you intended to create a blank RESTART file for the "
  //       "purpose of initiating restarts w/o specifying in the input yaml.\n"; //TODO: Place in generic output file instead?
  //   return pRI;
  // }

  // in.close();

  pRI.restartPopulated = true;
  pRI.validate();
  return pRI;
}
}
