#include "./smallsh.h"
#include "./printFunctions.h"

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
    // TODO get rid of this--go three times for starters
    int keepGoing = 3;
    while (keepGoing > 0) {
        // Get the user's input and build a command struct from it
        char* input = getRawInput();
        struct command* newCommand = createCommand(input);

        printStartTerminal();
        // If the input isn't blank, execute the instruction 
        if (newCommand->instruction) {
            // TODO Step 1 : let's just print things ok
            //executeCommand(command);
            printText(newCommand->instruction);
        }
        keepGoing--;

    }

    return EXIT_SUCCESS;
}