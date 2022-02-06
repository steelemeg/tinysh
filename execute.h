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
    // If debugging, show the resulting path after executing the cd.
    if (debugMessages) { 
        getcwd(cwdResults, MAX_COMMAND * 2 + 1);
        printShout(cwdResults, true); 
    }
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
*  The shell will print the process id of a background process when it begins.
*/
void execLibrary(struct command* currCommand) {
    // Per the assignment, fork a child process for non-built-in-commands.
    // Create a fresh pid (based on module code, full citation in readme)
    pid_t newPid = -5;
    int childExitStatus;
    bool redirectedInput = false; 
    bool redirectedOutput = false;
    bool childCreatedInBackground = false;
    // Per the assignment, we must print a message when background processes conclude. 
    char* bgExitMessage = calloc(MAX_ARG, sizeof(char));

    newPid = fork();
    // Based on the sample program execution, this message will be required for bg children
    sprintf(bgExitMessage, "background pid is %d", newPid);

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
        // Background and foreground children should ignore SIGTSTP. Since this case is child-only, set to ignore.
        handleSIGTSTP(false);

        // Are we redirecting input or output?
        if (currCommand->redirectOutput) { 
            redirector(currCommand->outputTarget, false, true); 
            redirectedOutput = true;
        }
        if (currCommand->redirectInput) { 
            redirector(currCommand->inputSource, true, false); 
            redirectedInput = true;
        }
        
        if (currCommand->backgroundJob && allowBackgroundMode) { 
            if (debugMessages) { printShout("Background job setup begins", true); }
            childCreatedInBackground = true;
            // Per the specs, any children running as background processes must ignore SIGINT.
            handleSIGINT(false);
            // per the spec, if no input redirect for a background command, then standard input should be redirected to /dev/null
            if (!redirectedInput) { redirector(NULL, true, false); }
            // per the spec, if no output redirect for a background command, then standard output should be redirected to /dev/null
            if (!redirectedOutput) { redirector(NULL, false, true); }
        }
        // If the command is foreground, OR if background mode is disabled, set up to run as a fg process. 
        // Per spec, child running as a foreground process must terminate itself when it receives SIGINT
        // When this occurs the parent must print out a message, which will be handled later.
        // TODO don't forget the message
        else { 
            if (debugMessages) { printShout("Foreground job setup begins", true); }
            childCreatedInBackground = false;
            handleSIGINT(true); 
        }

        // Child process created and tracked, signal handlers configured. Execute the actual command.
        // Viable options for executing library commands: 
        // execvp (wants an array), execlp (will take just strings but the last one should be null)
        // I want to use one of these two because they will look in the PATH for the command
        // Trying execvp because it seems easier to pass an existing array of char*s than each of the operands
        if (debugMessages) { printShout("Executing instruction with operands", true); }
        execvp(currCommand->instruction, currCommand->operands);
        break;
    }
    default:
        // Parent will execute the code in this branch 
        // For background commands, the shell must not wait for completion
        if (allowBackgroundMode && currCommand->backgroundJob) {
            if (debugMessages) { printShout("Background job parent messaging begins", true); }
            // Track the child in the linked list
            createChild(&firstChild, newPid);
            // The shell will print the process id of a background process when it begins,
            // has to go here to be executed by the parent shell     
            printShout(bgExitMessage, true);
            // Now we let it go off and do its thing in the background. 
        }

        // The shell must wait for the completion of foreground commands 
        else {
            if (debugMessages) { printShout("Foreground job parent messaging begins", true); }
            // TODO 
            printf("child pid was %d\n", newPid);
            fflush(stdout);
            // Copied from https://canvas.oregonstate.edu/courses/1884946/pages/exploration-shell-commands-related-to-processes
            newPid = waitpid(newPid, &childExitStatus, 0);
            if (debugMessages) { printShout("Foregroudn job parent, waitpid concludes", true); }
            // Based on https://linux.die.net/man/2/waitpid full citation in readme
            // waitpid should return WIFEXITED true if normal termination and and the actual exit status in WIFEXITSTATUS
            if (WIFEXITED(childExitStatus)) { lastFGExitStatus = WEXITSTATUS(childExitStatus); }
            // If WIFEXITED was false, then there was a problem. WTERMSIG will return the number of the signal that caused the 
            // child process to terminate.
            else if (WIFSIGNALED(childExitStatus)) { lastFGExitStatus = WTERMSIG(childExitStatus); }
            // Catchall for edge cases
            else { lastFGExitStatus = WTERMSIG(childExitStatus); }            
        }
    } 
    if (debugMessages) { printShout("Execution task concludes", true); }
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
    // TODO 
    //killZombieChildren();

    return;
}