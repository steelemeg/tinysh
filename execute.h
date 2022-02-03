// Functions for executing specific commands

/* 
*  Accepts no arguments. 
*  Kills any child processes or jobs.
*  Used as part of the exit function execution.
*  Returns the current exit success parameter value.
*/
int killChildProcesses() {
    // Per Ed #351 (formal citation in the readme)
    // TODO Walk through the array of child PIDs. Kill them if they are running.
    return EXIT_SUCCESS;
}

/*
*  Used to change the current working directory 
*  Accepts one parameter, a command struct, which allows us to get the relevant operands.
*  If no target directory is provided, goes to user home.
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
    
    else{ printShout(currCommand->instruction); }

    return;
}