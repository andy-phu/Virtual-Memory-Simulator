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
    int fifo;
    int lfu;
} Page;


int virtualMemory[VIRTUAL_MEMORY_SIZE] = {-1};
int mainMemory[MAIN_MEMORY_SIZE] = {-1};
Page ptable[NUM_VIRTUAL_PAGES];
int num_pages_used = 0;
int fifo_counter = 0; // the lower the number the longer it has been in main
int replacement_algorithim; //1 for FIFO and 2 for LRU

int findPage(int address){
    return (address/8);
}

int findAvailableMainMemoryPage(){
    if (num_pages_used < NUM_MAIN_PAGES){ // if there is available pages in main memory, insert into next
        return num_pages_used + 1;
    }
    else{
        if (replacement_algorithim == 1){ // FIFO Replacement
            int curr_longest_in_main_memory = 100000;
            for (int i = 0; i < NUM_VIRTUAL_PAGES; i++) {
                if(ptable[i].validBit == 1){
                    if(ptable[i].fifo <= curr_longest_in_main_memory){
                        curr_longest_in_main_memory = ptable[i].pageNumber;
                    }
                }
            }
            return curr_longest_in_main_memory;
        } 
        else{
            //LRU Replacement
        }
    }
}

//to find the main memory address that corresponds to the virtual memory address
int findMainMemoryAddress(int page_number, int virtual_address){
    return ((page_number * 8) + (virtual_address % 8));
}

int main(int argument, char* argv[]) {
    if (argument == 2) {
        char *algorithm = argv[1];
        if (strcmp(algorithm, "FIFO") == 0) { // Selected page replacement algorithm: FIFO
            printf("Selected page replacement algorithm: FIFO\n");
            replacement_algorithim = 1;
        } else if (strcmp(algorithm, "LRU") == 0){ // Selected page replacement algorithm: LRU
            printf("Selected page replacement algorithm: LRU\n");
            replacement_algorithim = 2;
        }
    } else{ //Default selected page replacement algorithm: FIFO
        printf("Selected page replacement algorithm: FIFO\n");
        replacement_algorithim = 1;
    }

    int address_counter = 0;
    for (int i = 0; i < NUM_VIRTUAL_PAGES; i++) {
        ptable[i].validBit = 0;
        ptable[i].dirtyBit = 0;
        ptable[i].pageNumber = -1;
        // for (int j = 0; j < PAGE_SIZE; j++) {
        //     ptable[i].addresses[j] = address_counter; // Initialize with -1
        //     address_counter++;
        // }
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
            
            if (ptable[corresponding_page].validBit == 0){
                printf("A Page Fault Has Occurred\n");
                printf("vPage: %d | vAdress: %d | Content: %d\n", corresponding_page, virtual_address, virtualMemory[virtual_address]);
            }
            else{
                int virtual_to_main = findMainMemoryAddress(ptable[corresponding_page].pageNumber, virtual_address);
                printf("Content: %d\n", mainMemory[virtual_to_main]);
            }
        }
        else if (strcmp(command,"write") == 0 && arg1 != NULL && arg2 != NULL){
            int virtual_address = atoi(arg1);
            int data = atoi(arg2);

            int corresponding_page = findPage(virtual_address);

            virtualMemory[virtual_address] = data; //writing to virtual memory


            if (ptable[corresponding_page].validBit == 0){
                printf("A Page Fault Has Occurred\n");
                printf("vPage: %d | vAdress: %d | Content: %d\n", corresponding_page, virtual_address, virtualMemory[virtual_address]);

                int available_page = findAvailableMainMemoryPage();

                int start_replacing_from = available_page * 8; //address to start replacing mainMemory data with virtualMemory data

                for(int i = corresponding_page * 8; i <= (corresponding_page * 8) + 8; i++){
                    mainMemory[start_replacing_from] = virtualMemory[i]; //replacing the page thus replacing 8 addresses
                    start_replacing_from++;
                }

                ptable[corresponding_page].validBit = 1;
                ptable[corresponding_page].pageNumber = available_page;
                ptable[corresponding_page].fifo = fifo_counter;

                num_pages_used++;
                fifo_counter++;
                
                


            } else{
                //go to page in mainMemory through ptable[corresponding_page].pageNumber and then add the data there
                if (ptable[corresponding_page].dirtyBit == 0){
                    ptable[corresponding_page].dirtyBit = 1;
                }
                int virtual_to_main = findMainMemoryAddress(ptable[corresponding_page].pageNumber, virtual_address);

                mainMemory[virtual_to_main] = data;
            }
        }
        else if (strcmp(command,"showptable") == 0){
            for(int i = 0; i < NUM_VIRTUAL_PAGES; i++){
                printf("%d:%d:%d:%d\n", i, ptable[i].validBit, ptable[i].dirtyBit, ptable[i].pageNumber);
            }

        }

    }
    return 0;
}