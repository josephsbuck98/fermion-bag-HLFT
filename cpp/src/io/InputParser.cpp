#include <sstream>
#include <yaml-cpp/yaml.h>

#include "InputParser.hpp"



namespace InputParser {
ParsedInput parseInputFile(const std::string& filename) {
  ParsedInput pI = YAML::LoadFile(filename).as<ParsedInput>();

  // Carry out any final changes
  pI.configurationInput.numTimeGroups = pI.controlInput.numTimeGroups;

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
      }
    } else if (title == "RANDOM_SEED") {
      if (std::getline(file, line)) {
        pRI.seed = std::stoi(line);
      }
    } else if (title == "RNG_STATE") {
      if (std::getline(file, line)) {
        std::istringstream iss(line);
        iss >> pRI.state;
      }
    } else if (title == "CONFIGURATION") { //Configuration must be last due to constraints with ifstream input reading
      file >> pRI.configuration;
      if (file.eof()) {
        std::cout << "SUCCESSFULLY READ IN CONFIGURATION" << std::endl;
      } else {
        std::cout << "Error reading in configuration." << std::endl;
        throw std::runtime_error("Error reading in configuration.");
      }
    } else {
      //TODO: Case where the title is garbage by some mistake. Throw error.
    }

  }

  file.close();

  //TODO: If validation was not 100%, throw an error. Probably don't need the restartPopulated variable

  pRI.restartPopulated = true;
  pRI.validate();
  return pRI;
}
}
