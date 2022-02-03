/* Author:					Megan Steele
* Last Modified:			02/05/2022
* OSU email address:		marshmeg@oregonstate.edu
* Course number/section:    CS344 Section 405
* Project Number:			3 (smallsh)
* Due Date:					02/07/2022
* Description:				TODO
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

#include "./parsing.h"
#include "./printing.h"
#include "./execute.h"


// Basic template copied from students.c Please see readme for full citations.
// If you are not compiling with the gcc option --std=gnu99, then
// uncomment the following line or you might get a compiler warning
//#define _GNU_SOURCE

/*
*   Runs an interactive shell program
*   Compile the program as follows:
*       gcc --std=gnu99 -o smallsh main.c
*/
int main(int argc, char* argv[]) {
    // TODO deal with SIGINT
    // TODO deal with SIGTST
    // TODO get rid of this--go three times for starters
    int keepGoing = 3;
    while (keepGoing > 0) {
        // Mimicing the formatting of the screenshots
        printStartTerminal();
        // Get the user's input and build a command struct from it
        char* input = getRawInput();
        struct command* newCommand = createCommand(input);

        
        // If the input isn't blank, execute the instruction 
        if (newCommand->instruction) {
            // TODO Step 1 : let's just print things ok
            //executeCommand(command);
            execCommand(newCommand);
        }
        keepGoing--;
    // TODO deal with child processes lurking about
    }

    return EXIT_SUCCESS;
}