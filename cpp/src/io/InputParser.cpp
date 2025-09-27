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
ParsedRestartInput parseRestartInputFile(std::string restartPath) { //TODO: Change input to std::filesystem::path
  ParsedRestartInput pRI;

  std::ifstream in(restartPath);
  if (!in) {
    pRI.restartPopulated = false;
    return pRI;
  }
  if (!(in >> pRI.currSweepId && in >> pRI.configuration && 
        in >> pRI.seed && in >> pRI.state)) {
    pRI.restartPopulated = false;
    std::cout << "WARNING: RESTART file corrupted or incomplete. Disregard "
        "this message if you intended to create a blank RESTART file for the "
        "purpose of initiating restarts w/o specifying in the input yaml.\n"; //TODO: Place in generic output file instead?
    return pRI;
  }

  pRI.restartPopulated = true;
  pRI.validate();
  return pRI;
}
}
