#include <iostream>

void runBondTests();
void runConfigurationTests();
void runLatticeTests();
void runInputTests(const std::string& filepath);

int main(int argc, char** argv) {
  std::string filepath = argv[1];
	
  std::cout << "<<<<<RUNNING ALL TESTS>>>>>\n\n";
  std::cout << argv[1] << std::endl;

  runBondTests();
  runConfigurationTests();
  runLatticeTests();
  runInputTests(filepath);
  return 0;
}
