// Functions for executing specific commands

/* The exit command exits your shell. 
*  First of the three built-in shell commands that smallsh handles. 
*  Accepts no arguments. 
*  Kills any child processes or jobs.
*/
int killChildProcesses() {
    // Per Ed #351 (formal citation in the readme)
    // TODO Walk through the array of child PIDs. Kill them if they are running.
    return EXIT_SUCCESS;
}

/*
* Executes instruction with arguments in the shell. Accepts a command struct containing parametersn
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
    
    else{ printShout(currCommand->instruction); }

    return;
}