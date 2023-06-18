#include <iostream>
#include <fstream>
#include <unordered_map>

const int PAGE_SIZE = 256;
const int FRAME_SIZE = 256;
const int TLB_SIZE = 16;
const int PAGE_TABLE_SIZE = 256;
const int PHYSICAL_MEMORY_SIZE = FRAME_SIZE * PAGE_TABLE_SIZE;

struct TLBEntry {
    int pageNumber;
    int frameNumber;
};

int getPageNumber(int logicalAddress) {
    return (logicalAddress >> 8) & 0xFF;
}

int getOffset(int logicalAddress) {
    return logicalAddress & 0xFF;
}

int readFromPhysicalMemory(int physicalAddress) {
    return rand() % 256;
}

int main() {
    std::ifstream logicalAddressesFile("logical_addresses.txt");
    std::ofstream physicalAddressesFile("physical_addresses.txt");
    std::unordered_map<int, int> pageTable;
    TLBEntry tlb[TLB_SIZE];
    int physicalMemory[PHYSICAL_MEMORY_SIZE];
    int tlbIndex = 0;
    int tlbHits = 0;
    int pageFaults = 0;
    int totalAddresses = 0;

    for (int i = 0; i < TLB_SIZE; i++) {
        tlb[i].pageNumber = -1;
        tlb[i].frameNumber = -1;
    }
    for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
        pageTable[i] = -1;
    }

    int logicalAddress;
    while (logicalAddressesFile >> logicalAddress) {
        totalAddresses++;
        int pageNumber = getPageNumber(logicalAddress);
        int offset = getOffset(logicalAddress);

        bool tlbHit = false;
        int frameNumber = -1;
        for (int i = 0; i < TLB_SIZE; i++) {
            if (tlb[i].pageNumber == pageNumber) {
                frameNumber = tlb[i].frameNumber;
                tlbHits++;
                tlbHit = true;
                break;
            }
        }

        if (!tlbHit) {
            auto pageTableEntry = pageTable.find(pageNumber);
            if (pageTableEntry != pageTable.end()) {
                frameNumber = pageTableEntry->second;
            } else {
                frameNumber = rand() % PAGE_TABLE_SIZE;
                pageTable[pageNumber] = frameNumber;
                pageFaults++;
            }

            tlb[tlbIndex].pageNumber = pageNumber;
            tlb[tlbIndex].frameNumber = frameNumber;
            tlbIndex = (tlbIndex + 1) % TLB_SIZE;
        }

        int physicalAddress = (frameNumber * PAGE_SIZE) + offset;
        int value = readFromPhysicalMemory(physicalAddress);

        physicalAddressesFile << physicalAddress << std::endl;

        std::cout << "Logical address: " << logicalAddress
                  << ", Physical address: " << physicalAddress
                  << ", Value: " << value << std::endl;
    }

    std::cout << "TLB Hits: " << tlbHits << std::endl;
    std::cout << "Page Faults: " << pageFaults << std::endl;
    std::cout << "Page Fault Rate: " << static_cast<double>(pageFaults) / totalAddresses << std::endl;

    return 0;
}
