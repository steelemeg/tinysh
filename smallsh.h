/* Author:					Megan Steele
* Last Modified:			02/05/2022
* OSU email address:		marshmeg@oregonstate.edu
* Course number/section:    CS344 Section 405
* Project Number:			3 (smallsh)
* Due Date:					02/07/2022
* Description:				This program reads in a specified csv, creates a subfolder, and populates it with files based on the contents
*                           of the specified csv. Note: Submitted second copy in revision period. Appropriate comment left on Canvas.
*/

// Basic template copied from students.c Please see readme for full citations.
// If you are not compiling with the gcc option --std=gnu99, then
// uncomment the following line or you might get a compiler warning
//#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <dirent.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>

// Defining some items for frequent use.
#define DELIMITER " "
// Your shell must support command lines with a maximum length of 2048 characters, and a maximum of 512 arguments.
#define MAX_COMMAND 2048
#define MAX_ARG 512

// As recommended in the assignment, creating a struct to hold commands and their possible parameters.
struct command {
    char* instruction;
    int operandCount;
    char** operands;
    bool isComment;
};

/*
* Accepts user input, creates a new command struct based on its contents
* Structure originally based on project 1 createMovie.
*/
struct command* createCommand(char* userInput) {
    char* saveptr;
    char* token;
    struct command* newCommand = malloc(sizeof(struct command));
    char* copyInput = calloc(strlen(userInput) + 1, sizeof(char));
    strcpy(copyInput, userInput);
    const char comment = '#';

    // The first token should be the instruction. It should also tell us if this is a blank or comment.
    token = strtok_r(userInput, DELIMITER, &saveptr);
    // Handle bad inputs. Inner calloc/strcpy lifted from Project 1, if statements protect from blanks and comments.
    if (token != NULL) {
        // Ignore comments
        if (token[0] != comment) {
            newCommand->instruction = calloc(strlen(token) + 1, sizeof(char));
            strcpy(newCommand->instruction, token);
        }
    }
    return newCommand;
}

/*
* Prints a specified string to the terminal
* Be sure you flush out the output buffers each time you print!
*/
void printText(const char* printTarget) {
    printf("%s", printTarget);
    fflush(NULL);

    return;
}

/* Get raw user input */
char* getRawInput() {
    char* input = calloc(MAX_COMMAND, sizeof(char));
    // using fgets instead of getline based on project 2 testing
    fgets(input, MAX_COMMAND, stdin);
    // Drop the trailing newline
    input[strcspn(input, "\n")] = 0;
    // TODO something with expansion, pain, etc
    return input;
}