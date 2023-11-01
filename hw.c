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
    return (address/8);
}

Page virtualMemory[NUM_VIRTUAL_PAGES];
Page mainMemory[NUM_MAIN_PAGES];

int main() {
    // Initialize virtual memory
    int address_counter = 0;
    for (int i = 0; i < NUM_VIRTUAL_PAGES; i++) {
        virtualMemory[i].validBit = 0;
        virtualMemory[i].dirtyBit = 0;
        virtualMemory[i].pageNumber = i;
        for (int j = 0; j < PAGE_SIZE; j++) {
            virtualMemory[i].addresses[address_counter] = -1; // Initialize with -1
            address_counter++;
        }
        //printf("initilization valid bit for page (%d): %d\n", i, virtualMemory[i].validBit);
    }

    address_counter = 0;
    // Initialize main memory
    for (int a = 0; a < NUM_MAIN_PAGES; a++) {
        mainMemory[a].validBit = 0;
        mainMemory[a].dirtyBit = 0; // Assume initially not modified
        mainMemory[a].pageNumber = a;
        for (int b = 0; b < PAGE_SIZE; b++) {
            mainMemory[a].addresses[address_counter] = -1; // Initialize with -1
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

            //printf("valid bit: %d\n", virtualMemory[0].validBit);
            if (virtualMemory[corresponding_page].validBit == 0){ //since main memory only has pages 0-3 accessing outside of that would be a page fault
                printf("A Page Fault Has Occurred\n");
                printf("vPage: %d | vAdress: %d | Content: %d\n", corresponding_page, virtual_address, virtualMemory[corresponding_page].addresses[virtual_address]);
            }
        }

    }
    return 0;
}