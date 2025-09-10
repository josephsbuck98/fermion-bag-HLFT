#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Input.hpp"
#include "Output.hpp"



Output::Output(InputParser::ParsedInput input, std::string inFileName) {
  this->input = input;
  outDir = fs::current_path() / input.outputInput.outDirName;

  // Create or verify creation of outDir
  createOutDir(); 

  // Generate initial output files
  createOutFiles(input.outputInput.outDirName, inFileName);

  // Write the header
  //TODO: Only write the header if either restarts=false, or restarts=true AND RESTART is not present.
  writeHeader();

  // Initialize the vector of Sweeps
  Sweep sweepPlaceholder = Sweep(input, -1); // Only here to facilitate build
  sweepsCache.resize(input.outputInput.outSweepsPatience, sweepPlaceholder);

}

void Output::createOutDir() {
  if ((!fs::exists(outDir)) && (!fs::create_directory(outDir))) {
    // Directory doesn't exist and wasn't created successfuly
    throw std::runtime_error("Failed to create directory: " + outDir.string());
  } else if (!input.outputInput.restarts) { 
    // Not restarting and outDir is present
    try {
      for (const auto& entry : fs::directory_iterator(outDir)) {
        fs::remove_all(entry);
      }
    } catch (const fs::filesystem_error& err) {
      throw std::runtime_error(std::string("Error clearing directory: ") 
          + err.what());
    }
  }
  // Otherwise, directory already exists (& restarts) or was created 
}

void Output::createOutFiles(std::string outDirName, std::string inFileName) {
  bool throwError = false;
  
  std::string stdOutPostfix = keyFromValue<std::string, consts::OutFileType>
      (consts::OUTFILE_TYPE_MAP, consts::OutFileType::STD_OUT_POSTFIX);
  fs::path stdOutPath = outDir / (outDirName + stdOutPostfix);
  if (!fs::exists(stdOutPath)) {
    std::ofstream ofs(stdOutPath);
    if (!ofs) throwError = true;
  }

  std::string sweepsFileName = keyFromValue<std::string, consts::OutFileType>
      (consts::OUTFILE_TYPE_MAP, consts::OutFileType::SWEEPS);
  fs::path sweepsDatPath = outDir / sweepsFileName;
  if (!fs::exists(sweepsDatPath)) {
    std::ofstream ofs(sweepsDatPath);
    if (!ofs) throwError = true;
  }

  std::string bondsPerTypeFileName = 
      keyFromValue<std::string, consts::OutFileType>
      (consts::OUTFILE_TYPE_MAP, consts::OutFileType::BONDS_PER_TYPE);
  fs::path bondsPerTypeDatPath = outDir / bondsPerTypeFileName;
  if (!fs::exists(bondsPerTypeDatPath)) {
    std::ofstream ofs(bondsPerTypeDatPath);
    if (!ofs) throwError = true;
  }

  std::string inputGenericFileName = 
      keyFromValue<std::string, consts::OutFileType>
      (consts::OUTFILE_TYPE_MAP, consts::OutFileType::INPUT);
  fs::copy_file(fs::current_path() / inFileName, 
      outDir / inputGenericFileName, fs::copy_options::overwrite_existing);
  
  if (throwError) throw std::runtime_error("Outfile Creation Error: Some of "
      "the output files could not be created");
}



// Functions to store and write sweep data
void Output::storeSweep(Sweep newSweep) {
  //TODO: Set the next elem of vector to newSweep. Indexed using newSweep.id
  //TODO: If the index you just input at the end of the vector (in other words,
  //TODO: if newSweep.id % outSweepsPatience == sweepsCache.size()), call a 
  //TODO: func to write out the data in the vector to the dat files. Use input
  //TODO: to determine which files to write to. Clear data in sweepsCache.
}

void Output::writeAndClearSweepCache() {
  //TODO: Write to the different dat files by calling writeSweepsLine and 
  //TODO: writeBondsPerTypeLine. Use input to determine what to write.
}



// Functions to write out and read restart files
void Output::writeRestartFile() { // Make sure you only write restarts if restarts is true OR RESTART is present (even if empty. user can add it during a simulation to switch to writing out restarts.)
  //TODO: Implement RESTART file write outs
}

void Output::readRestartFile(Configuration configuration) {
  //TODO: Implement checks for RESTART presence. If not present, start from 
  //TODO: scratch. If present, load into configuration, return start_sweep, and
  //TODO: delete RESTART. Ensure startSweeps is less than maxSweeps. Throw 
  //TODO: prior to deleting restart file if reading was unsuccessful.
}



// Standard write-out functions
void Output::writeHeader() {
  std::ostringstream header;

  // Timestamp
  std::time_t now = std::time(nullptr);
  char buf[64];
  std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

  header << createSeparator(60, '-');
  header << createCenteredTitle(60, "Fermion Bag HLFT Output", '-');
  header << createSeparator(60, '-');
  header << "\n";
  header << "Date: " << buf << "\n";
  header << createParameterString(input);
  header << "\n";
  header << createSeparator(60, '-');

}

void Output::writeSweepStart() {

}

void Output::writeSweepEnd() {
  // Include an estimate for how long it will take.
}

void Output::writeDecadeReport() {

}

void Output::writeFinalReport() {

}


//TODO: Dat file writeouts
void Output::writeSweepsLine() {

}

void Output::writeBondsPerTypeLine() {

}


//TODO: Private functions to act as helpers
std::string Output::createSeparator(int len, char character) {
  std::string separator(len, character);
  separator += "\n";
  return std::string(len, character) + "\n";
}

std::string Output::createCenteredTitle(int lineLen, 
    const std::string& title, char spacer) {
  int titleLen = static_cast<int>(title.size()) + 2; // Add 2 for spaces
  if (titleLen > lineLen) return title;
  int padding = lineLen - titleLen;
  int left = padding / 2; int right = padding - left;
  std::string line = std::string(left, spacer) + " " + title + " " + 
      std::string(right, spacer);
  if (line.size() > lineLen) {
    return line.substr(0, lineLen) + "\n";
  } else if (line.size() < lineLen) {
    return line + std::string(lineLen - line.size(), spacer) + "\n";
  }
  return line + "\n";
}

std::string Output::createParameterString(const InputParser::ParsedInput& input) {
  std::ostringstream paramStr; //TODO: Make a print function in each of those input classes (overload << ?)

  paramStr << "Important Parameters:\n";
  paramStr << input.controlInput;
  paramStr << input.outputInput;
  paramStr << input.latticeInput;
  paramStr << input.hamiltonianInput;
  paramStr << input.configurationInput;

  return paramStr.str();
}