// Functions for executing specific commands

/* 
*  Accepts no arguments. 
*  Kills any child processes or jobs.
*  Used as part of the exit function execution.
*  A simplified version of killZombieChildren as it just sigterms everything.
*  Does not modify status flag per spec requirements.
*  Returns the current exit success parameter value.
*/
int killChildProcesses() {
    // Per Ed #351 (formal citation in the readme)
    // Walk through the linked list of child PIDs. Kill them if they are running.
    struct child* currChild = firstChild;
    char* output = calloc(50, sizeof(char));

    while (currChild != NULL)
    {
        kill(currChild->childPid, SIGKILL);
        sprintf(output, "background pid %d is done: terminated by signal 15", currChild->childPid);
        printShout(output, true);
        currChild = currChild->next;
    }
    free(output);
    free(currChild);
    return EXIT_SUCCESS;
}

/*
*  Used to change the current working directory 
*  Accepts one parameter, a command struct, which allows us to get the relevant operands.
*  If no target directory is provided, goes to user home.
*  Does not modify status flag per spec requirements.
*/
void execCd(struct command* currCommand) {
    // If I am understanding properly we aren't supposed to use getenv(PWD) but can use getcwd to check paths.
    // Per https://eklitzke.org/path-max-is-tricky, the theoretical max path length is 4096. Formal citation
    // in the readme.
    char* cwdResults = calloc(MAX_COMMAND * 2, sizeof(char));

    // Handle the command with no further arguments and go to the user's home dir
    if (currCommand->operandCount == 1 || !currCommand->operands[1]) {
        // Function pulled from Linux Programming Interface text, page 363-364; full citation in readme.
        chdir(getenv("HOME"));
    }
    else {
        // Try to change the current directory to the specified path
        char* newDir;
        // TODO 
        newDir = currCommand->operands[1];
        if (getcwd(cwdResults, MAX_COMMAND * 2 + 1) == NULL) { printError("Tried to get cwd, encountered error:"); }
        
        // First make sure the path isn't blank, which it really shouldn't be, but be safe
        if (newDir) {
            sprintf(cwdResults, "%s%s%s", cwdResults, "/", newDir);
            // Then see if the path exists.
            if (chdir(cwdResults) == -1) { printShout("No such file or directory.", true); }
            // If it exists, chdir into it.
            else { chdir(newDir); }
        }
        
    }
    getcwd(cwdResults, MAX_COMMAND * 2 + 1);
    if (debugMessages) { printShout(cwdResults); }
    free(cwdResults);
    return;
}

/*
*  Displays the current value of the exit status flag.
*  Accepts a command struct containing parameters.
*  Does not modify status flag per spec requirements.
*  Returns no values.
*/
void execStatus(struct command* currCommand) {
    char* output = calloc(20, sizeof(char));
    sprintf(output, "exit value %d", statusFlag);
    printShout(output, true);
    free(output);
    return;
}

/*
*  TODO
*  
*/
void execLibrary(struct command* currCommand) {
    // Per the assignment, fork a child process for non-built-in-commands.
    // Create a fresh pid (based on module code, full citation in readme)
    pid_t newPid = -5;
    // The max possible PID is somewhere between 32768 and 2^22 per https://stackoverflow.com/questions/6294133/maximum-pid-in-linux 
    // Formal citation in readme.
    // Going with a max allowed of 11 to be on the safe side, adding 1 for null terminator
    // TODO do we need this here? It's not like expansion
    char* newPidStr = calloc(12, sizeof(char));
    // Per the assignment, we must print a message when background processes conclude. 
    char* bgExitMessage = calloc(MAX_ARG, sizeof(char));
    //TODO get rid of this
    execvp(currCommand->instruction, currCommand->operands);
    /*newPid = fork();
    // Base code taken from https://canvas.oregonstate.edu/courses/1884946/pages/exploration-shell-commands-related-to-processes
    // Full citation in the readme
    switch (newPid) {
    case -1: {
        // Child process creation failed
        printError("fork() failed! No child process was created:");
        exit(1);
        break;
    }
    case 0: {
        // Child process creation successful. This code will be executed only by the child.
        // TODO SIGTSTP
        
        // Are we redirecting input or output?
        if (currCommand->redirectOutput) { redirector(currCommand->outputTarget, false, true); }
        else if (currCommand->redirectInput) { redirector(currCommand->inputSource, true, false); }
        
        // TODO background mode check
        if (currCommand->backgroundJob && allowBackgroundMode) {  }
        // Per the specs, any children running as background processes must ignore SIGINT, but a child running as a 
        // foreground process must terminate itself when it receives SIGINT
        // TODO implement this
        else {}
        // TODO if the child shoudl run in the background and is permitted to do so, redirect to dir/null
        // Viable options for executing library commands: 
        // execvp (wants an array), execlp (will take just strings but the last one should be null)
        // I want to use one of these two because they will look in the PATH for the command
        // Trying execvp because it seems easier to pass an existing array of char*s than each of the operands
        execvp(currCommand->instruction, currCommand->operands);
        break;
    }
    default:
        // Parent will execute the code in this branch TODO what pid is pid ugh
        sprintf(newPidStr, "%d", newPid);
        
        break;
    } */
    free(newPidStr);
    free(bgExitMessage);
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
    else if (strcmp(currCommand->instruction, "cd") == 0) { execCd(currCommand); }
    else if (strcmp(currCommand->instruction, "status") == 0) { execStatus(currCommand); }
    else{ execLibrary(currCommand); }
    killZombieChildren();

    return;
}