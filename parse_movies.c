#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//struct for individual movies
struct Movie {

    char* title;
    int year;
    char language[5][21];
    double rating;
    struct Movie* next;

};

//function that takes line of file and returns Movie struct
struct Movie *createMovie(char* currLine) {

    struct Movie *currMovie = malloc(sizeof(struct Movie));

    //different deliminators (inner for array)
    const char outer_delim[] = ",\n";
    const char inner_delim[] = "];[";

    char* token;
    char* endptr;
    char* outer_saveptr; //for strtok_r
    char* inner_saveptr; //for strtok_r

    //first token is title (string)
    token = strtok_r(currLine, outer_delim, &outer_saveptr);
    currMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->title, token);

    //second token is year (integer)
    token = strtok_r(NULL, outer_delim, &outer_saveptr);
    currMovie->year = atoi(token);

    //third token is language (array)
    token = strtok_r(NULL, outer_delim, &outer_saveptr);
    char* inner_token = strtok_r(token, inner_delim, &inner_saveptr);

    int i = 0;
    //adding languages to an array
    while (inner_token != NULL) {
        strcpy(currMovie->language[i], inner_token);
        inner_token = strtok_r(NULL, inner_delim, &inner_saveptr);
        i++;
    }

    //fourth token is rating (double)
    token = strtok_r(NULL, outer_delim, &outer_saveptr);
    currMovie->rating = strtod(token, &endptr); //don't know abt endptr

    //initializing next struct in linked list as NULL
    currMovie->next = NULL;

    return currMovie;
}

//function for processing file and making linked list of structs
struct Movie *makeLinkedList(char* filePath) {
    FILE *movieFile = fopen(filePath, "r"); //open file as read-only

    //initialization
    char* currLine = NULL;
    size_t len = 0;
    ssize_t nread;
    char* token;

    struct Movie* head = NULL; //head of linked list
    struct Movie* tail = NULL; //tail of linked list

    //read file line by line
    bool skip_first_line = true;
    int counter = 0;
    while ((nread = getline(&currLine, &len, movieFile)) != -1) {
        
        //skips general info line
        if (skip_first_line) {
            skip_first_line = false;
            continue;
        }

        //new node created for current line
        struct Movie* newNode = createMovie(currLine);
        counter++;

        //checks if first node in list
        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    free(currLine);
    fclose(movieFile);
    return head;
}
