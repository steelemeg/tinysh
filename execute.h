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
    struct child* currChild = firstChild;
    char* output = calloc(50, sizeof(char));
    if (childNum > 0){
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
*/
void execCd(struct command* currCommand) {
    // handle the command with no argument
    if (currCommand->operandCount == 0) {
        // Functions pulled from Linux Programming Interface text, page 363-364; full citation in readme.
        chdir(getenv("HOME"));
    }
    else {
        // Try to change the current directory to the specified path
        char* newDir;
        // TODO 
        newDir = currCommand->operands[1];
        // Per https://eklitzke.org/path-max-is-tricky, the theoretical max path length is 4096. Formal citation
        // in the readme.
        char* cwdResults = calloc(MAX_COMMAND * 2, sizeof(char));
        getcwd(cwdResults, sizeof(cwdResults));
        printf(cwdResults);
        
        // First make sure the path isn't blank, which it really shouldn't be, but be safe
        if (newDir) {
            sprintf(cwdResults, "%s%s%s", cwdResults, "/", newDir);
            printf(cwdResults);
            // Then see if the path exists.
            if (chdir(cwdResults) == -1) { printShout("No such file or directory."); }
            // If it exists, chdir into it.
            else { chdir(currCommand->operands[0]); }
        }
        free(cwdResults);
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
    // Per the assignment, fork a child process for non-built-in-commands.
    // Create a fresh pid (based on module code, full citation in readme)
    //TODO pid_t newPid = -5;
    // Viable options for executing library commands: 
    // execvp (wants an array), execlp (will take just strings but the last one should be null)
    // I want to use one of these two because they will look in the PATH for the command
    // Trying execvp because it seems easier to pass an array than each of the operands
    char* arguments[currCommand->operandCount];
    // Need a pointer to step through the array
    int buildArgs = 0;
    execvp(currCommand->instruction, currCommand->operands);
    //execlp(currCommand->instruction, currCommand->instruction, NULL);

    /* WOrry about basic exec first then do the background foreground madness TODO 
    newPid = fork();
    switch (newPid) {
    case -1: {
        // Child process creation failed
        printShout("fork() failed! No child process was created.");
        exit(1);
        break;
    }
    case 0: {
        // Child process creation successful. This code will be executed only by the child.
        // TODO SIGTSTP
        // TODO ugh gonna need a new array
        //execvp(currCommand->instruction, currCommand->operands[0]);
        break;
    }
    default:
        // Parent will execute the code in this branch
        break;
    } */
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

    return;
}