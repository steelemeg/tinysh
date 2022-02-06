// Contains functions for signal handling, redirection, and anything else that supports other function types.

/*
* Use dup2 to redirect input or output.
* Accepts three parameters, the name of the file, a bool indicating if this is an input redirect, and a bool indicating 
* if this is an output redirect.
* Returns no values.
* Based on code from https://canvas.oregonstate.edu/courses/1884946/pages/exploration-processes-and-i-slash-o, full citation in readme
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
		return 1;
	}
	// Per module example, close file descriptor on execution
	fcntl(fileD, F_SETFD, FD_CLOEXEC);
	// If all is well, do the actual redirect. Use dup2 to point to fileD, use the flag to determine if it's input or output
	result = dup2(fileD, dupFlag);
	// Handle dup2 errors 
	if (result == -1) {
		printError("Problem with redirection, dup2() execution generated an error");
		return 1;
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
	// Per the spec, we must display information messages when processes are terminated
	// These messages must take the form of either "background pid #### is done: exit value #" or 
	// "background pid #### is done: terminated by signal ##"
	char* informativeMessage = calloc(MAX_ARG, sizeof(char));
	struct child* currChild = firstChild;

	while (currChild != NULL)
	{
		// todo oh oby
		// todo reread https://canvas.oregonstate.edu/courses/1884946/pages/exploration-process-api-monitoring-child-processes
		//todo seperaate logic for background that ends itself and sigterm
		kill(currChild->childPid, SIGKILL);
		sprintf(informativeMessage, "background pid %d is done: terminated by signal 15", currChild->childPid);
		printShout(informativeMessage, true);

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
void handleSIGINT(bool dfl) {
	// Per Ed #387, need double braces to de-confuse gcc. Citation in readme.
	struct sigaction SIGINT_action = { { 0 } };
	// SIG_DFL – specifying this value means we want the default action to be taken for the signal type.
	if (dfl) { 
		if (debugMessages) {write(STDOUT_FILENO, "Handing sigint, default behavior\n", 33); }
		SIGINT_action.sa_handler = SIG_DFL; 
	}
	// Can we use SIG_IGN? Module makes it sound like yes
	else { 
		if (debugMessages) { write(STDOUT_FILENO, "Handing sigint, ignore behavior\n", 32); }
		SIGINT_action.sa_handler = SIG_IGN; 
	}

	// Block all catchable signals while handle_SIGINT is running
	sigfillset(&SIGINT_action.sa_mask);
	// No flags set
	SIGINT_action.sa_flags = 0;

	// Install our signal handler
	sigaction(SIGINT, &SIGINT_action, NULL);
	return;
}

/*
* TODO OBVIOUSLY
* Custom handler for SIGTSTP. 
* Accepts one parameter, the signal number 
* Based on code from https://canvas.oregonstate.edu/courses/1884946/pages/exploration-signal-handling-api
* Prints required messaging, toggles background mode flga
* Returns no values
*/
void customSIGTSTP(int signo) { 
	// Per the spec, need to print messages of the format "Entering foreground-only mode (& is now ignored)"
	// or "Exiting foreground-only mode"
	char* backgroundNoMessage = calloc(49, sizeof(char));
	sprintf(backgroundNoMessage, "Entering foreground-only mode (& is now ignored)\n");
	char* backgroundYesMessage = calloc(29, sizeof(char));
	sprintf(backgroundYesMessage, "Exiting foreground-only mode\n");

	// Using write per the module--printShout depends on printf, which is not re-entrant. 
	// Per the spec, if the parent process receives SIGTSTP, we must show a custom message
	// and disable background processing. If SIGTSTP is received again, another message must
	// be displayed and background processing should be re-enabled. Using the background mode
	// global flag to determine which behavior should be executed.
	if (allowBackgroundMode) { 
		write(STDOUT_FILENO, backgroundNoMessage, 49);
		allowBackgroundMode = false;
	}
	else if (!allowBackgroundMode) {
		write(STDOUT_FILENO, backgroundYesMessage, 29);
		allowBackgroundMode = true;
	}
	free(backgroundNoMessage);
	free(backgroundYesMessage);
	return; }

/*
* Handler for SIGTSTP. Per the spec, we must have a custom handler for SIGTSTP.
* Accepts a boolean specifying if default (true) or ignore (false)
* Based on module code from https://canvas.oregonstate.edu/courses/1884946/pages/exploration-signal-handling-api?module_item_id=21835981
* Since the sa_handler wants a function, this is effectively broken into two parts, handleSIGTSTP and custom
* Returns no values.
*/
void handleSIGTSTP(bool dfl) {
	// Per Ed #387, need double braces to de-confuse gcc. Citation in readme.
	struct sigaction SIGTSTP_action = { { 0 } };
	//SIG_DFL – specifying this value means we want the default action to be taken for the signal type.
	if (dfl) { 
		if (debugMessages) { write(STDOUT_FILENO, "Setting SIGTSTP to default\n", 27); }
		SIGTSTP_action.sa_handler = customSIGTSTP; 
	}
	else { 
		if (debugMessages) { write(STDOUT_FILENO, "Setting SIGTSTP to ignore\n", 26); }
		SIGTSTP_action.sa_handler = SIG_IGN; 
	}

	// Block all catchable signals while handle_SIGINT is running
	sigfillset(&SIGTSTP_action.sa_mask);
	// No flags set
	SIGTSTP_action.sa_flags = 0;

	// Install our signal handler
	sigaction(SIGTSTP, &SIGTSTP_action, NULL);
	return;
}