#pragma once

#include <filesystem>

#include "Configuration.hpp"
#include "InputParser.hpp"
#include "Sweep.hpp"


namespace fs = std::filesystem;

class Output {
public:
  Output(InputParser::ParsedInput input, std::string inFileName);

  void createOutDir(); //TODO: Private
  void createOutFiles(std::string outDirName, std::string inFileName); //TODO: Private

  void storeSweep(Sweep newSweep);
  void writeAndClearSweepCache(); //TODO: Private

  void writeRestartFile();
  void readRestartFile(Configuration configuration);

  void writeHeader();
  void writeSweepStart();
  void writeSweepEnd();
  void writeDecadeReport();
  void writeFinalReport();
  
  void writeSweepsLine();
  void writeBondsPerTypeLine();

private:
  InputParser::ParsedInput input;
  fs::path outDir;
  std::vector<Sweep> sweepsCache;
  
};