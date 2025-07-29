#include <iostream>

void runBondTests();
void runConfigurationTests();
void runLatticeTests();

int main(int argc, char** argv) {
    std::cout << "<<<<<RUNNING ALL TESTS>>>>>\n\n";
    runBondTests();
    runConfigurationTests();
    runLatticeTests();
    return 0;
}
