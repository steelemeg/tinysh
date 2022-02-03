// Functions for executing specific commands

/* The exit command exits your shell. 
*  First of the three built-in shell commands that smallsh handles. 
*  Accepts no arguments. 
*  Kills any child processes or jobs.
*/
void execExit() {
    // Per Ed #351 (formal citation in the readme)
    // TODO Walk through the array of child PIDs. Kill them if they are running.

    // Call the built-in shell exit with the status flag
    //exit(EXIT_SUCCESS);
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
    else if (strcmp(currCommand->instruction, "exit") == 0) { execExit(); }
    
    else{ printShout(currCommand->instruction); }

    return;
}