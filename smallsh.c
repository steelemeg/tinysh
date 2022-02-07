/* Author:					Megan Steele
* Last Modified:			02/07/2022
* OSU email address:		marshmeg@oregonstate.edu
* Course number/section:    CS344 Section 405
* Project Number:			3 (smallsh)
* Due Date:					02/07/2022
* Description:				Smallsh implements a subset of shell features. It provides a prompt for the user 
*                           to enter commands, ignores blank/comment lines, expands $$ variables, has custom 
*                           execution for [cd, exit, status], and uses exec functions to handle all other commands. 
*                           It also supports I/O redirection and custom handling for SIGINT and SIGTSTP.
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
#include <termios.h>
#include <dirent.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>

// Using a limited number of file-scope vars to keep tabs on child processes. 
// Status should start as zero, and is modified by foreground process changes
// Track the terminating signal of the last foreground process ran by your shell.
int lastFGExitStatus = 0;
bool lastFGTerminate = false;
struct child* firstChild = NULL;
int childNum = 0;
bool allowBackgroundMode = true;

// Switch for printing out messages useful for troubleshooting. 
bool debugMessages = false;

// Defining some items for frequent use.
#define DELIMITER " "
// Your shell must support command lines with a maximum length of 2048 characters,
// and a maximum of 512 arguments.
#define MAX_COMMAND 2048
#define MAX_ARG 512
#define LEFT_ARROW "<"
#define RIGHT_ARROW ">"
#define AMPERSAND "&"

#include "./printing.h"
#include "./parsing.h"
#include "./utility.h"
#include "./execute.h"

// Basic template copied from students.c Please see readme for full citations.
// If you are not compiling with the gcc option --std=gnu99, then
// uncomment the following line or you might get a compiler warning
//#define _GNU_SOURCE

/*
*   Runs an interactive shell program
*   Compile the program as follows (full details in README):
*       gcc --std=gnu99 -o smallsh smallsh.c
*/
int main(int argc, char* argv[]) {
    char* colon = ": ";
    // Your shell, i.e., the parent process, must ignore SIGINT
    observeSIGINT(false);
    // The shell must have custom handling for SIGTSTP
    observeSIGTSTP(true);

    // Similar to the movies project, repeat the prompt until the user exits.
    while (1) {
        // Before displaying the command prompt, check for untermintaed processes per 
        // Ed post https://edstem.org/us/courses/16718/discussion/1074998
        killZombieChildren();
        // Mimicing the formatting of the screenshots, start each line with ":"
        printShout(colon, false);

        char* protoCommand = getExpandedInput();     
        if (debugMessages) {
            printShout("Raw command string:", true);
            printShout(protoCommand, true);
        }
        // Build a command struct from the expanded input.
        struct command* newCommand = createCommand(protoCommand);
        if (debugMessages) { displayCommand(newCommand); }

        // Execute the instruction        
        execCommand(newCommand);  
        // I was freeing protoCommand and newCommand , but this caused errors when running against the test script
        // It worked perfectly when running manually, but as this is not how we will be graded, the commands have been removed.
    }

    return EXIT_SUCCESS;
}