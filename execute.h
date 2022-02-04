// Functions for executing specific commands

/* 
*  Accepts no arguments. 
*  Kills any child processes or jobs.
*  Used as part of the exit function execution.
*  Does not modify status flag.
*  Returns the current exit success parameter value.
*/
int killChildProcesses() {
    // Per Ed #351 (formal citation in the readme)
    // Walk through the linked list of child PIDs. Kill them if they are running.
    if (childNum > 0){
        struct child* currChild = firstChild;
        char* output = calloc(50, sizeof(char));
        while (currChild != NULL)
        {
            kill(currChild->childPid, SIGKILL);
            sprintf(output, "Background process with pid %d terminated", currChild->childPid);
            printShout(output);
            currChild = currChild->next;
        }
    }
    free(output);
    free(currChild);
    return EXIT_SUCCESS;
}

/*
*  Used to change the current working directory 
*  Accepts one parameter, a command struct, which allows us to get the relevant operands.
*  If no target directory is provided, goes to user home.
*  Does not modify status flag.
*  Based on my admittedly limited understanding of chdir, it supports relative and absolute paths natively.
*/
void execCd(struct command* currCommand) {
    // handle the command with no argument
    if (currCommand->operandCount == 0) {
        // Functions pulled from Linux Programming Interface text, page 363-364; full citation in readme.
        chdir(getenv("HOME"));
    }
    else {
        // Try to change the current directory to the specified path
        // First make sure it exists.
        if (chdir(currCommand->operands[0]) == -1) { printShout("No such file or directory."); }
        // If it exists, chdir into it.
        else { chdir(currCommand->operands[0]); }
    }

    return;
}

/*
*  Displays the current value of the exit status flag.
*  Accepts a command struct containing parameters.
*  Does not modify status flag.
*  Returns no values.
*/
void execStatus(struct command* currCommand) {
    char* output = calloc(20, sizeof(char));
    sprintf(output, "exit value %d\n", statusFlag);
    printShout(output);
    free(output);
    return;
}

/*
*  TODO
*  
*/
void execLibrary(struct command* currCommand) {
}
/*
* Executes instruction with arguments in the shell. 
* Accepts a command struct containing parameters
* Calls the appropriate function based on the instruction and arguments in the command struct.
* Returns no values.
*/
void execCommand(struct command* currCommand) {

    // Comments mean we should do nothing! 
    if (currCommand->isCommentOrBlank) {
        //pass
    }
    // actual commands
    else if (strcmp(currCommand->instruction, "exit") == 0) { 
        int success = killChildProcesses();
        // Need to use the built-in shell exit command unlike previous projects where we just did a "return EXIT_SUCCESS"
        // Kill parent process to break the calling function's while loop. 
        //https://www.tutorialspoint.com/c_standard_library/c_function_exit.htm formal citation in readme
        exit(success);
    }
    
    else if (strcmp(currCommand->instruction, "cd") == 0) { execCd(currCommand); }
    else if (strcmp(currCommand->instruction, "status") == 0) { execStatus(currCommand); }
    else{ printShout(currCommand->instruction); }

    return;
}