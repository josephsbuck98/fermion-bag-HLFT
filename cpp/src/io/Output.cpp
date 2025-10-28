#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

#include "Input.hpp"
#include "Output.hpp"
#include "RandomHelpers.hpp"



Output::Output(InputParser::ParsedInput input, std::string inFileName, 
    Configuration& configuration) : configuration(configuration) {
  this->input = input;
  outDir = fs::current_path() / input.outputInput.outDirName;

  // Create or verify creation of outDir
  createOutDir(); 

  // Generate initial output files
  createOutFiles(input.outputInput.outDirName, inFileName);

  // Write the headers for standard outfile and dat files
  fs::path restartPath = getRestartPath();
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
        fs::remove_all(entry); //TODO: Input parameter to force removal. If not true, throw error
      }
    } catch (const fs::filesystem_error& err) {
      throw std::runtime_error(std::string("Error clearing directory: ") 
          + err.what());
    }
  }
  // Otherwise, directory already exists (& restarts) or was created 
}

void Output::createOutFiles(std::string outDirName, std::string inFileName) {
  bool throwError = false; //TODO: Replace keyFromValue with funcs at very bottom
  
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
    sweepsCache[0].setId(-1); // Purpose is so cache isn't double outputted
  }

  if (fs::exists(getRestartPath())) input.outputInput.restarts = true;
}

void Output::writeAndClearSweepCache() {
  if (sweepsCache[0].getId() == -1) return;
  int lastIter;
  for (int i = 0; i < sweepsCache.size(); i++) {
    lastIter = i;
    writeSweepsLine(sweepsCache[i]);
    if (input.outputInput.writeBondsPerType) { 
      writeBondsPerTypeLine(sweepsCache[i]); //TODO: THIS IS NOT WORKING FOR SOME REASON
    }
    if (input.controlInput.maxSweeps - 1 == sweepsCache[i].getId() ||
        sweepsCache.size() - 1 == i) {
      break;
    }
  }
  if (input.outputInput.restarts) {
    writeRestartFiles(sweepsCache[lastIter].getId());
  }
}



// Functions to write out and read restart files
void Output::writeRestartFiles(int currSweepId) {
  std::cout << "Writing restart file..." << std::endl;

  std::ofstream restartStream(getRestartPath());
  if (!restartStream) {
    // Log failure to standard output (not output file)
    std::cout << "Failed to write RESTART file at sweep " << 
      currSweepId << std::endl;
    return;
  }

  std::mt19937_64& rng = globalRNG();

  restartStream << "[[CURR_SWEEP_ID]]\n" << currSweepId << "\n";
  
  restartStream << "[[RANDOM_SEED]]\n" << input.controlInput.randomSeed << "\n";
  restartStream << "[[RNG_STATE]]\n" << rng << "\n";

  restartStream << configuration << "\n";

}



// Standard write-out functions
void Output::writeHeader() {
  std::ostringstream header;

  //TODO: IF RESTARTS IS SELECTED, OUTPUT A MESSAGE SAYING YOU MUST USE THE EXACT SAME INPUT YAML AS THE ORIGINAL RUNS.

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
    std::cout << key << " " << value << std::endl;
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
  int width = 7; //TODO: Define this width once for each file type.
  std::ostringstream line;
  line << std::left;

  int iter = 1;
  for (const auto& [key, value] : sweep.getNumBondsPerType()) {
    for (int i = iter; i < key; i++) {
      line << std::setw(width) << 0;
    }
    line << std::setw(width) << value;
    iter = key + 1;
  }
  for (int i = iter; i < 6; i++) {
      line << std::setw(width) << 0;
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

std::string Output::getRestartPath() {
  return outDir / keyFromValue<std::string, consts::OutFileType>
      (consts::OUTFILE_TYPE_MAP, consts::OutFileType::RESTART);
}