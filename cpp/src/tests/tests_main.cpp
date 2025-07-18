#include <iostream>

void runBondTests();
void runConfigurationTests();

int main(int argc, char** argv) {
    std::cout << "<<<<<RUNNING ALL TESTS>>>>>\n\n";
    runBondTests();
    runConfigurationTests();
    return 0;
}
