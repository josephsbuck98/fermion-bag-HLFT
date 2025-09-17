#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "Input.hpp"
#include "Output.hpp"



Output::Output(InputParser::ParsedInput input, std::string inFileName, 
    Configuration& configuration) : configuration(configuration) {
  this->input = input;
  outDir = fs::current_path() / input.outputInput.outDirName;

  // Create or verify creation of outDir
  createOutDir(); 

  // Generate initial output files
  createOutFiles(input.outputInput.outDirName, inFileName);

  // Write the headers for standard outfile and dat files
  fs::path restartPath = 
      outDir / keyFromValue<std::string, consts::OutFileType>
      (consts::OUTFILE_TYPE_MAP, consts::OutFileType::RESTART);
  if (!(input.outputInput.restarts && fs::exists(restartPath))) {
    writeHeader();
    writeSweepsHeader();
    if (input.outputInput.writeBondsPerType) {
      writeBondsPerTypeHeader();
    }
  }

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
void Output::storeSweep(Sweep sweep) {
  int outSweepsPatience = input.outputInput.outSweepsPatience;
  int index = sweep.getId() % outSweepsPatience;
  sweepsCache[index] = std::move(sweep);

  if (index == outSweepsPatience - 1) {
    writeAndClearSweepCache();
    if (input.outputInput.restarts) {
      writeRestartFile(); //TODO: Pass in configuration and iteration + 1. 
    }
  }
}

void Output::writeAndClearSweepCache() { 
  for (int i = 0; i < sweepsCache.size(); i++) { 
    writeSweepsLine(sweepsCache[i]);
    if (input.outputInput.writeBondsPerType) { 
      writeBondsPerTypeLine(sweepsCache[i]); //TODO: THIS IS NOT WORKING FOR SOME REASON
    }
    if (input.controlInput.maxSweeps - 1 == sweepsCache[i].getId()) {
      break;
    }
  }
}



// Functions to write out and read restart files
//TODO: Allow user to simply create a RESTART file in the outdir during a run, and it will start writing RESTARTs 
void Output::writeRestartFile() {
  //TODO: Call a configuration public member function to write out the configuration, and pass in the next id.
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
  auto now = std::chrono::system_clock::now();
  std::time_t nowC = std::chrono::system_clock::to_time_t(now);

  header << createSeparator(60, '-');
  header << createCenteredTitle(60, "Fermion Bag HLFT Output", '-');
  header << createSeparator(60, '-');
  header << "\n";
  header << "Date: " << std::put_time(std::localtime(&nowC), 
      "%Y-%m-%d %H:%M:%S") << "\n";
  header << createParameterString(input);
  header << "\n";
  header << createSeparator(60, '-');
  header << "\n"; // Extra line after separator

  std::ofstream outFile(outDir / getStdOutFilename(), std::ios::trunc);
  outFile << header.str();
}

void Output::writeSweepsHeader() {
  std::ostringstream header;
  header << "sweepID  ";
  header << "numTimeGroups  ";
  header << "numRejects  ";
  header << "numInserts  ";
  header << "numRemoves  ";
  header << "numBonds  ";
  header << "\n";
  header << createSeparator(80, '-');
  
  std::ofstream outFile(outDir / getSweepsFilename(), std::ios::trunc);
  outFile << header.str();
}

void Output::writeBondsPerTypeHeader() {
  int width = 7;
  std::ostringstream header;
  header << std::left;
  for (const auto& [key, value] : input.hamiltonianInput.bondTypeProps) {
    header << std::setw(width) << key;
  }
  header << "\n";
  header << createSeparator(60, '-');

  std::ofstream outFile(outDir / getBondsPerTypeFilename(), std::ios::trunc);
  outFile << header.str();
}

void Output::writeSweepBegin() {

}

void Output::writeSweepEnd() {
  // Include an estimate for how long it will take.
}

void Output::writeDecadeReport() {

}

void Output::writeFinalReport() {

}


// Dat file write-outs
void Output::writeSweepsLine(const Sweep& sweep) {
  int width = 7;
  std::ostringstream line;
  line << std::left;
  line << std::setw(width) << sweep.getId();
  line << std::setw(width) << sweep.getFinNumTimeGroups();
  line << std::setw(width) << sweep.getNumRejects();
  line << std::setw(width) << sweep.getNumInserts();
  line << std::setw(width) << sweep.getNumRemoves();
  line << std::setw(width) << sweep.getNumBonds();
  line << "\n";

  std::ofstream outFile(outDir / getSweepsFilename(), std::ios::app);
  outFile << line.str();
}

void Output::writeBondsPerTypeLine(const Sweep& sweep) {
  int width = 7;
  std::ostringstream line;
  line << std::left;
  for (const auto& [key, value] : sweep.getNumBondsPerType()) {
    line << std::setw(width) << value;
  }
  line << "\n";

  std::ofstream outFile(outDir / getBondsPerTypeFilename(), std::ios::app);
  outFile << line.str();
}


// Private helper functions
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
  std::ostringstream paramStr; 

  paramStr << "Important Parameters:\n";
  paramStr << input.controlInput;
  paramStr << input.outputInput;
  paramStr << input.latticeInput;
  paramStr << input.hamiltonianInput;
  paramStr << input.configurationInput;

  return paramStr.str();
}

std::string Output::getStdOutFilename() {
  std::string outDirName = input.outputInput.outDirName;
  std::string stdOutPostfix = keyFromValue<std::string, consts::OutFileType>
      (consts::OUTFILE_TYPE_MAP, consts::OutFileType::STD_OUT_POSTFIX);
  return outDirName + stdOutPostfix;
}

std::string Output::getSweepsFilename() {
  return keyFromValue<std::string, consts::OutFileType>
      (consts::OUTFILE_TYPE_MAP, consts::OutFileType::SWEEPS);
}

std::string Output::getBondsPerTypeFilename() {
  return keyFromValue<std::string, consts::OutFileType>
      (consts::OUTFILE_TYPE_MAP, consts::OutFileType::BONDS_PER_TYPE);
}