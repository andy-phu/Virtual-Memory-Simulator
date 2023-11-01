#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int findPage(int address){
    int i;
    int startCounter = 0; //to keep track of the starting address index for each page
    int endCounter = 7;
    for (i = 0; i < 16; i++){ //to find what virtual page the virtual address is on 
        if (address >= startCounter && address <= endCounter){ //if its between the 8 addreses range 
            break;
        }
        startCounter = endCounter + 1;
        endCounter+=8;
    }
    return i;
}

int main() {
    // Initialize virtual memory
    Page virtualMemory[NUM_VIRTUAL_PAGES];
    int address_counter = 0;
    for (int i = 0; i < NUM_VIRTUAL_PAGES; i++) {
        virtualMemory[i].validBit = 0;
        virtualMemory[i].dirtyBit = 0;
        virtualMemory[i].pageNumber = i;
        for (int j = 0; j < PAGE_SIZE; j++) {
            virtualMemory[i].addresses[address_counter] = -1; // Initialize with -1
            address_counter++;
        }
    }

    address_counter = 0;
    // Initialize main memory
    Page mainMemory[NUM_MAIN_PAGES];
    for (int i = 0; i < NUM_MAIN_PAGES; i++) {
        mainMemory[i].validBit = 0;
        mainMemory[i].dirtyBit = 0; // Assume initially not modified
        mainMemory[i].pageNumber = i;
        for (int j = 0; j < PAGE_SIZE; j++) {
            mainMemory[i].addresses[address_counter] = -1; // Initialize with -1
            address_counter++;
        }
    }


    //3.2
    char input[128];

    while(1){
        printf("> ");
        
        if (fgets(input, 128, stdin) == NULL) {
            perror("Failed to read input");
        }

        char *command = strtok(input, " \n");
        char *arg1 = NULL; 
        char *arg2 = NULL; 


        if (command != NULL){
            arg1 = strtok(NULL, " \n");        
        }

        if (arg1!= NULL){
            arg2 = strtok(NULL, " \n");        
        }

    
        if(strcmp(command,"quit") == 0){
            break;
        }
        else if (strcmp(command,"read") == 0 && arg1 != NULL){
            int virtual_address = atoi(arg1);

            int corresponding_page = findPage(virtual_address);
            printf("%d", virtualMemory[corresponding_page].validBit);
            if (virtualMemory[corresponding_page].validBit == 0){ //since main memory only has pages 0-3 accessing outside of that would be a page fault
                printf("A Page Fault Has Occurred\n");
                printf("Virtual Page: %d Content: %d\n", virtual_address, virtualMemory[corresponding_page].addresses[virtual_address]);
            }
        }

    }
    return 0;
}