#include <iostream>

void runBondTests();
void runConfigurationTests();

int main(int argc, char** argv) {
    std::cout << "Running all tests...\n";
    runBondTests();
    runConfigurationTests();
    return 0;
}
