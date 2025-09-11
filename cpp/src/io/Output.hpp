#pragma once

#include <filesystem>

#include "Configuration.hpp"
#include "InputParser.hpp"
#include "Sweep.hpp"


namespace fs = std::filesystem;

class Output {
public:
  Output(InputParser::ParsedInput input, std::string inFileName, 
      Configuration& configuration);

  void createOutDir(); //TODO: Private
  void createOutFiles(std::string outDirName, std::string inFileName); //TODO: Private

  void storeSweep(Sweep newSweep);
  void writeAndClearSweepCache(); //TODO: Private

  void writeRestartFile();
  void readRestartFile(Configuration configuration);

  void writeHeader();
  void writeSweepsHeader();
  void writeBondsPerTypeHeader();

  void writeSweepBegin();
  void writeSweepEnd();
  void writeDecadeReport();
  void writeFinalReport();
  
  void writeSweepsLine(const Sweep& sweep);
  void writeBondsPerTypeLine(const Sweep& sweep);

private:
  std::string createSeparator(int len, char character);
  std::string createCenteredTitle(int lineLen, 
      const std::string& title, char spacer);
  std::string createParameterString(const InputParser::ParsedInput& input);
  std::string getStdOutFilename();
  std::string getSweepsFilename();
  std::string getBondsPerTypeFilename();
  
  Configuration& configuration;
  fs::path outDir;
  InputParser::ParsedInput input;
  std::vector<Sweep> sweepsCache;
  
};