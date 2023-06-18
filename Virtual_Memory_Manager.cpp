#include <iostream>
#include <fstream>
#include <unordered_map>

// Constants
const int PAGE_SIZE = 256; // Page size in bytes
const int FRAME_SIZE = 256; // Frame size in bytes
const int TLB_SIZE = 16; // TLB size
const int PAGE_TABLE_SIZE = 256; // Page table size
const unsigned int PHYSICAL_MEMORY_SIZE = FRAME_SIZE * PAGE_TABLE_SIZE; // Physical memory size

// TLB entry structure
struct TLBEntry {
    int pageNumber;
    unsigned int frameNumber; // Changed data type to unsigned int
};

// Function to extract page number from logical address
int getPageNumber(int logicalAddress) {
    return (logicalAddress >> 8) & 0xFF;
}

// Function to extract offset from logical address
int getOffset(int logicalAddress) {
    return logicalAddress & 0xFF;
}

// Function to simulate reading a value from physical memory
int readFromPhysicalMemory(unsigned int physicalAddress) { // Changed data type to unsigned int
    // Here you can implement the actual reading from physical memory
    // For simplicity, let's assume the physical memory contains random values
    return rand() % 256;
}

int main() {
    // Variables
    std::ifstream logicalAddressesFile("logical_addresses.txt");
    std::ofstream physicalAddressesFile("physical_addresses.txt"); // File to write physical addresses
    std::unordered_map<int, unsigned int> pageTable; // Changed data type to unsigned int
    TLBEntry tlb[TLB_SIZE];
    int physicalMemory[PHYSICAL_MEMORY_SIZE];
    int tlbIndex = 0;
    int tlbHits = 0;
    int pageFaults = 0;
    int totalAddresses = 0;

    // Initialize TLB and page table
    for (int i = 0; i < TLB_SIZE; i++) {
        tlb[i].pageNumber = -1;
        tlb[i].frameNumber = -1;
    }
    for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
        pageTable[i] = -1;
    }

    // Read logical addresses from file
    int logicalAddress;
    while (logicalAddressesFile >> logicalAddress) {
        totalAddresses++;

        // Extract page number and offset
        int pageNumber = getPageNumber(logicalAddress);
        int offset = getOffset(logicalAddress);

        // Check if the page number is present in the TLB
        bool tlbHit = false;
        unsigned int frameNumber = -1; // Changed data type to unsigned int
        for (int i = 0; i < TLB_SIZE; i++) {
            if (tlb[i].pageNumber == pageNumber) {
                frameNumber = tlb[i].frameNumber;
                tlbHits++;
                tlbHit = true;
                break;
            }
        }

        // If TLB miss, check the page table
        if (!tlbHit) {
            // Check if the page number is present in the page table
            auto pageTableEntry = pageTable.find(pageNumber);
            if (pageTableEntry != pageTable.end()) {
                frameNumber = pageTableEntry->second;
            } else {
                // Page fault, generate a random frame number and update the page table
                frameNumber = rand() % PAGE_TABLE_SIZE;
                pageTable[pageNumber] = frameNumber;
                pageFaults++;
            }

            // Update the TLB with the new page number and frame number
            tlb[tlbIndex].pageNumber = pageNumber;
            tlb[tlbIndex].frameNumber = frameNumber;
            tlbIndex = (tlbIndex + 1) % TLB_SIZE;
        }

        // Calculate the physical address and read the value from physical memory
        unsigned int physicalAddress = (frameNumber * PAGE_SIZE) + offset; // Changed data type to unsigned int
        int value = readFromPhysicalMemory(physicalAddress);

        // Write the physical address to the file
        physicalAddressesFile << physicalAddress << std::endl;

        // Output the result
        std::cout << "Logical address: " << logicalAddress << ", Physical address: " << physicalAddress << ", Value: " << value << std::endl;
    }

    // Output statistics
    std::cout << "TLB Hits: " << tlbHits << std::endl;
    std::cout << "Page Faults: " << pageFaults << std::endl;
    std::cout << "Page Fault Rate: " << static_cast<double>(pageFaults) / totalAddresses << std::endl;

    return 0;
}
