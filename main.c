#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "parse_movies.c"

#define PREFIX "movie"


//creates txt files and appends movie names to them
void createFiles(struct dirent* filePtr, char* dirName) {
    DIR* newDir = opendir(dirName);
    //calls function from parse_movies.c to make movies linked list used from
    //assignment one
    struct Movie* list = makeLinkedList(filePtr->d_name);

    while (list != NULL) { 
        //converts integer to string and concatonates string for file name
        char syear[5];
        sprintf(syear, "%d", list->year);
        char* ext = ".txt";

        char* fileName = malloc(strlen(syear) + strlen(ext) + 1);
        strcpy(fileName, syear);
        strcat(fileName, ext);
        
        //makes and writes to file with year.txt
        char buffer[10000];
        snprintf(buffer, sizeof(buffer), "%s/%s", dirName, fileName);
        FILE* fptr = fopen(buffer, "a");
        fprintf(fptr, "%s\n", list->title);
        chmod(buffer, S_IRUSR | S_IWUSR | S_IRGRP);
        fclose(fptr);

        list = list->next; //iteration
    }
    closedir(newDir);
}


//makes directory with ONID + movies + random number
void processFile(struct dirent* filePtr) {
    printf("\nNow processing the chosen file named %s.\n", filePtr->d_name);
    
    srand(time(NULL)); //ensures random number

    //calculates random number and converts it into a string
    char srandnum[6];
    int randnum = rand() % 100000;
    sprintf(srandnum, "%d", randnum);
    char* partDirName = "sheahinl.movies.";
    
    //reserves space for directory name string and concatonates strings
    char* dirName = malloc(strlen(partDirName) + strlen(srandnum) + 1);
    strcpy(dirName, partDirName);
    strcat(dirName, srandnum);
    
    int status = mkdir(dirName, 0750); //makes new directory
    if (status == 0) {
        printf("Created directory with name %s\n\n", dirName);
    } else {
        perror("Error creating directory.");
    }

    createFiles(filePtr, dirName);
}


//function if largest file option is picked
void largestFile() {
    struct dirent* filePtr; //pointer for file iteration
    DIR* currDir = opendir(".");
    struct stat dirStat;
    struct dirent* largestFile;
    off_t largestFileSize = 0;
    
    //while loop code based off of code from exploration directories
    while ((filePtr = readdir(currDir)) != NULL) {
        if (strncmp(PREFIX, filePtr->d_name, strlen(PREFIX)) == 0) {
            stat(filePtr->d_name, &dirStat); //grabs metadata for entry
        }

        char* point; 
        if((point = strrchr(filePtr->d_name,'.')) != NULL ) {
            if(strcmp(point,".csv") == 0) { //checks if file ends in .csv
                //saves current largest file size
                if (largestFileSize <= dirStat.st_size) {
                    largestFileSize = dirStat.st_size;
                    largestFile = filePtr;
                }
            }
        }
    }
    closedir(currDir);
    processFile(largestFile);
}


//function if smallest file option is picked
void smallestFile() {
    struct dirent* filePtr; //pointer for file iteration
    DIR* currDir = opendir(".");
    struct stat dirStat;
    struct dirent* smallestFile;
    off_t smallestFileSize = 922337204000000000;
    
    //while loop code based off of code from exploration directories
    while ((filePtr = readdir(currDir)) != NULL) {
        if (strncmp(PREFIX, filePtr->d_name, strlen(PREFIX)) == 0) {
            stat(filePtr->d_name, &dirStat); //grabs metadata for entry
        }

        char* point; 
        if((point = strrchr(filePtr->d_name,'.')) != NULL ) {
            if(strcmp(point,".csv") == 0) { //checks if file ends in .csv
                //saves current smallest file size
                if (smallestFileSize >= dirStat.st_size) {
                    smallestFileSize = dirStat.st_size;
                    smallestFile = filePtr;
                }
            }
        }
    }
    closedir(currDir);
    processFile(smallestFile);
}


//function if specific file option is picked
bool specificFile() {
    char* answer;
    printf("\nEnter the complete file name: ");
    scanf("%s", answer);

    struct dirent* filePtr;
    DIR* currDir = opendir(".");
    bool notFoundDir = true; //error handling
    
    //checks files for match of user input
    while ((filePtr = readdir(currDir)) != NULL) {
        if (strcmp(filePtr->d_name, answer) == 0) {
            notFoundDir = false;
            closedir(currDir);
            processFile(filePtr);
            break;
        }
    }
    //executes if file not found in current directory
    if (notFoundDir) {
        printf("\nInputed file name does not exist. Please try again\n");
        closedir(currDir);
        return false;
    }
    return true;
}


//function for printing the menu for choices
void printMenu() {
    bool running = true;
    while (running) {
        int answer;
        printf("\nWhich file do you want to process?\n");
        printf("Enter 1 to pick the largest file\n");
        printf("Enter 2 to pick the smallest file\n");
        printf("Enter 3 to specify the name of a file\n");
        printf("Enter a choice from 1 to 3: ");
        scanf("%i", &answer);

        if (answer == 1) {
            largestFile();
            running = false;
        } else if (answer == 2) {
            smallestFile();
            running = false;
        } else if (answer == 3) {
            bool dontrun = specificFile();
            if (dontrun) {
                running = false;
            }
        } else {
            //executes if invalid input
            printf("\nInvalid input. Please try again.");
        }
    }
}


//main function. prints first menu.
int main() {
    bool running = true;
    while (running) {
        int answer;
        printf("1. Select file to process\n");
        printf("2. Exit the program\n");
        printf("Enter a choice 1 or 2: ");
        scanf("%i", &answer);

        if (answer == 1) {
            printMenu();
        } else if (answer == 2) {
            running = false;
        } else {
            //executes if invalid input
            printf("\nInvalid input. Please try again.\n\n");
        }
    }
    exit(EXIT_SUCCESS);
}
