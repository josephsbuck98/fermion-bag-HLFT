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
  bool createOutFile(std::string outFileName, consts::OutFileType fileType);

  void storeSweep(Sweep newSweep, int startSweepId);
  void writeAndClearSweepCache(); //TODO: Private

  void writeRestartFile(int currSweepId);

  void writeHeader();
  void writeSweepsHeader();
  void writeBondsPerTypeHeader();

  void writeSweepBegin();
  void writeSweepEnd();
  void writeDecadeReport();
  void writeFinalReport();
  
  void writeSweepsLine(const Sweep& sweep);
  void writeBondsPerTypeLine(const Sweep& sweep);

  std::string getRestartPath(); //TODO: Make this return std::filesystem::path

  private:
  std::string createSeparator(int len, char character); //TODO: Reorganize these so they are grouped by common structure actions
  std::string createCenteredTitle(int lineLen, 
      const std::string& title, char spacer);
  std::string createParameterString(const InputParser::ParsedInput& input);
  std::string getStdOutFilename();
  std::string getSweepsFilename();
  std::string getGenericFilename();
  std::string getBondsPerTypeFilename();
  
  Configuration& configuration;
  fs::path outDir;
  InputParser::ParsedInput input;
  std::vector<Sweep> sweepsCache;
  
};