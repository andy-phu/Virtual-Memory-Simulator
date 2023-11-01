#include <stdio.h>

#define VIRTUAL_MEMORY_SIZE 128
#define MAIN_MEMORY_SIZE 32
#define PAGE_SIZE 8
#define NUM_VIRTUAL_PAGES (VIRTUAL_MEMORY_SIZE / PAGE_SIZE)
#define NUM_MAIN_PAGES (MAIN_MEMORY_SIZE / PAGE_SIZE)

// Define a structure to represent a page
typedef struct {
    int validBit;
    int dirtyBit;
    int pageNumber;
    int addresses[PAGE_SIZE];
} Page;

int main() {
    // Initialize virtual memory
    Page virtualMemory[NUM_VIRTUAL_PAGES];
    for (int i = 0; i < NUM_VIRTUAL_PAGES; i++) {
        virtualMemory[i].validBit = 0;
        virtualMemory[i].dirtyBit = 0;
        virtualMemory[i].pageNumber = i;
        for (int j = 0; j < PAGE_SIZE; j++) {
            virtualMemory[i].addresses[j] = -1; // Initialize with -1
        }
    }

    // Initialize main memory
    Page mainMemory[NUM_MAIN_PAGES];
    for (int i = 0; i < NUM_MAIN_PAGES; i++) {
        mainMemory[i].validBit = 0;
        mainMemory[i].dirtyBit = 0; // Assume initially not modified
        mainMemory[i].pageNumber = i;
        for (int j = 0; j < PAGE_SIZE; j++) {
            mainMemory[i].addresses[j] = -1; // Initialize with -1
        }
    }

    return 0;
}