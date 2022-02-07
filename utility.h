// Contains functions for signal handling, redirection, and anything else that supports other function types.

/*
* Use dup2 to redirect input or output.
* Accepts three parameters, the name of the file, a bool indicating if this is an input redirect, and a bool indicating 
* if this is an output redirect.
* Returns no values.
* Based on code from https://canvas.oregonstate.edu/courses/1884946/pages/exploration-processes-and-i-slash-o, 
* full citation in readme
*/
int redirector(char* targetFile, bool input, bool output) {
	// Modifying lecture code to account for the possibility of a blank filename. 
	// Per assignment if filename is blank, use /dev/null
	char* filename;
	int fileD = 0;
	int result = 0;
	int dupFlag = input ? 0 : 1;

	if (input && output) {
		printError("Invalid use of function redirector");
		return 1;
	}
	if (targetFile != NULL) { filename = targetFile; }
	else {
		filename = "/dev/null";
	}
	// If this is input, the file should already exist. If it's output, it might not.
	if (input) { fileD = open(filename, O_RDONLY); }
	else if (output) { fileD = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644); }

	// Handle file opening errors
	if (fileD == -1) {
		printError("Problem opening file: ");
		// Originally returned, which caused serious problems when the filename was not valid.
		// Changing based on sample code from https://canvas.oregonstate.edu/courses/1884946/pages/exploration-processes-and-i-slash-o
		exit(1);
	}

	// If all is well, do the actual redirect. Use dup2 to point to fileD, use the flag to determine if it's input or output
	result = dup2(fileD, dupFlag);
	// Per module example, close file descriptor on execution
	fcntl(fileD, F_SETFD, FD_CLOEXEC);
	// Handle dup2 errors 
	if (result == -1) {
		printError("Problem with redirection, dup2() execution generated an error");
		exit(2);
	}
	return 0;
}

/*
* Based on the assignment, we need to ignore some signals (i.e. a child running as a backgroupd process must ignore SIGTSTP).
* Per the signal handling module, we also have to define a sa_handler for a given sigaction struct
* Creating a pass-through function so it can be provided in these scenarios.
* Accepts one parameter to mimic the handler examples in module (https://canvas.oregonstate.edu/courses/1884946/pages/exploration-signal-handling-api)
* Returns no values.
* TODO Might not need this?
*/
void ignoreSignal(int signo) {
	// pass
	//TODO 
	printShout("ignore time", true);
	return;
}

/* 
* TODO clean up the mess after an instruction executes
* TODO already called after instruction switch
*/
void killZombieChildren() {
	pid_t pid;
	int currentPid;
	// Per the spec, we must display information messages when processes are terminated
	// These messages must take the form of either "background pid #### is done: exit value #" or 
	// "background pid #### is done: terminated by signal ##"
	char* informativeMessage = calloc(MAX_ARG, sizeof(char));
	struct child* currChild = firstChild;
	int* statusFlag = malloc(sizeof(int));

	while (currChild != NULL)
	{
		// todo reread https://canvas.oregonstate.edu/courses/1884946/pages/exploration-process-api-monitoring-child-processes
		//todo seperaate logic for background that ends itself and sigterm
		// Per the module, use the WNOHANG option to see if any child process has just ended
		currentPid = currChild->childPid;
		currentPid = waitpid(currentPid, statusFlag, WNOHANG);

		// on success, waitpid returns the pid of the child who state has changed
		if (currentPid > 0) {
			if (WIFEXITED(*statusFlag)) {
				sprintf(informativeMessage, "background pid %d is done: exit value %d", currentPid, *statusFlag);
			
				printShout(informativeMessage, true);
				removeChild(&firstChild, currentPid);
			}

			// If WNOHANG was specified and one or more child(ren) specified by pid exist, but have not yet changed state, then 0 is returned.
			// If there is an error, -1 is returned.
			else {
				sprintf(informativeMessage, "background pid %d is done: terminated by signal %d", currentPid, WTERMSIG(*statusFlag));
				printShout(informativeMessage, true);
				removeChild(&firstChild, currentPid);
			}
		}
		// todo handle removing child
		currChild = currChild->next;
	}

	free(informativeMessage);
	return;
}
/* 
* Handler for SIGINT. Per the spec, we must have a custom handler for SIGINT.
* Accepts a boolean specifying if default (true) or ignore (false)
* Based on module code from https://canvas.oregonstate.edu/courses/1884946/pages/exploration-signal-handling-api?module_item_id=21835981
* Returns no values.
*/



/*
* Handler for SIGTSTP. Per the spec, we must have a custom handler for SIGTSTP.
* Accepts a boolean specifying if default (true) or ignore (false)
* Based on module code from https://canvas.oregonstate.edu/courses/1884946/pages/exploration-signal-handling-api?module_item_id=21835981
* Since the sa_handler wants a function, this is effectively broken into two parts, handleSIGTSTP and custom
* Returns no values.
*/

void observeSIGINT(bool dfl) {
	// Per Ed #387, need double braces to de-confuse gcc. Citation in readme.
	struct sigaction SIGINT_action = { { 0 } };
	// SIG_DFL – specifying this value means we want the default action to be taken for the signal type.
	if (dfl) { SIGINT_action.sa_handler = SIG_DFL; }
	else { SIGINT_action.sa_handler = SIG_IGN; }


	// Block all catchable signals while handle_SIGINT is running
	sigfillset(&SIGINT_action.sa_mask);
	// No flags set
	SIGINT_action.sa_flags = 0;

	// Install our signal handler
	sigaction(SIGINT, &SIGINT_action, NULL);
	return;
}

/*
* Custom handler for SIGTSTP.
* Accepts one parameter, the signal number
* Based on code from https://canvas.oregonstate.edu/courses/1884946/pages/exploration-signal-handling-api
* Prints required messaging, toggles background mode flag -- every time a SIGTSTP is received by a process
*	using this handler, it won't kill but will toggle the flag.
* Returns no values
*/
void customSIGTSTP(int signo) {
	char backgroundTurningOff[50] = "Entering foreground-only mode (& is now ignored)\n";		// length 49
	char backgroundTurningOn[30] = "Exiting foreground-only mode\n";							// length 29

	// Using write per the module--printShout depends on printf, which is not re-entrant. 
	// Per the spec, if the parent process receives SIGTSTP, we must show a custom message
	// and disable background processing. If SIGTSTP is received again, another message must
	// be displayed and background processing should be re-enabled. Using the background mode
	// global flag to determine which behavior should be executed.

	// Per Ed, do not use strlen in handler. Also don't use fflush. TODO https://edstem.org/us/courses/16718/discussion/1075111
	if (allowBackgroundMode) { write(STDOUT_FILENO, backgroundTurningOff, 49); }
	else { write(STDOUT_FILENO, backgroundTurningOn, 29); }
		
	// FLIP the allowBackgroundMode flag
	if (allowBackgroundMode) { allowBackgroundMode = false; }
	else { allowBackgroundMode = true; }
	return;

}

void observeSIGTSTP(bool dfl) {
	// Per Ed #387, need double braces to de-confuse gcc. Citation in readme.
	struct sigaction SIGTSTP_action = { { 0 } };

	if (debugMessages) { 
		write(STDOUT_FILENO, "Setting SIGTSTP\n", 27);
	}
	if (dfl) { SIGTSTP_action.sa_handler = customSIGTSTP; }
	else { SIGTSTP_action.sa_handler = SIG_IGN; }

	// Block all catchable signals while handle_SIGINT is running
	sigfillset(&SIGTSTP_action.sa_mask);
	// No flags set
	SIGTSTP_action.sa_flags = 0;

	// Install our signal handler
	sigaction(SIGTSTP, &SIGTSTP_action, NULL);
	return;
}


