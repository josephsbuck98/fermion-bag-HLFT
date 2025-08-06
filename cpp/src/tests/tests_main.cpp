#include <iostream>

void runBondTests();
void runConfigurationTests();
void runLatticeTests();
void runInputTests();

int main(int argc, char** argv) {
    std::cout << "<<<<<RUNNING ALL TESTS>>>>>\n\n";
    runBondTests();
    runConfigurationTests();
    runLatticeTests();
    runInputTests();
    return 0;
}
