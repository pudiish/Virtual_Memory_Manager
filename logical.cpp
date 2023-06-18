#include <iostream>
#include <fstream>
#include <random>

int main() {
    std::ofstream logicalAddressesFile("logical_addresses.txt");

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 65535); // Range of logical addresses

    const int NUM_ADDRESSES = 10; // Number of logical addresses to generate

    for (int i = 0; i < NUM_ADDRESSES; i++) {
        int logicalAddress = dis(gen);
        logicalAddressesFile << logicalAddress << std::endl;
    }

    logicalAddressesFile.close();

    std::cout << "Logical addresses have been generated and stored in logical_addresses.txt." << std::endl;

    return 0;
}
