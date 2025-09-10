#include <fstream>
#include <iostream>

#include "Output.hpp"



Output::Output(InputParser::ParsedInput input, std::string inFileName) {
  this->input = input; //TODO: Just pass in and use outputInput?
  outDir = fs::current_path() / input.outputInput.outDirName;

  // Create or verify creation of outDir
  createOutDir(); 

  // Generate initial output files
  createOutFiles(input.outputInput.outDirName, inFileName);

  // Initialize the vector of Sweeps
  Sweep sweepPlaceholder = Sweep(input, 0); // Only here to facilitate build
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
  
  //TODO: All of these file name strings (including the .out postfix) should be enum constants
  fs::path stdOutPath = outDir / (outDirName + ".out");
  if (!fs::exists(stdOutPath)) {
    std::ofstream ofs(stdOutPath); 
    if (!ofs) throwError = true;
  }

  fs::path sweepsDatPath = outDir / "sweeps.dat";
  if (!fs::exists(sweepsDatPath)) {
    std::ofstream ofs(sweepsDatPath);
    if (!ofs) throwError = true;
  }

  fs::path bondsPerTypeDatPath = outDir / "bonds-per-type.dat";
  if (!fs::exists(bondsPerTypeDatPath)) {
    std::ofstream ofs(bondsPerTypeDatPath);
    if (!ofs) throwError = true;
  }

  fs::copy_file(fs::current_path() / inFileName, outDir / "INPUT", 
      fs::copy_options::overwrite_existing);
  
  if (throwError) throw std::runtime_error("Outfile Creation Error: Some of "
      "the output files could not be created");
}



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
  //TODO: delete RESTART. Ensure start_sweeps is less than max_sweeps. Throw 
  //TODO: prior to deleting restart file if reading was unsuccessful.
}



//TODO: Standard write-outs of sweep starts, time usage, progress, equilibration data, etc. 
void Output::writeHeader() {
  // Include an estimate for how long it will take.
}

void Output::writeSweepStart() {

}

void Output::writeSweepEnd() {

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