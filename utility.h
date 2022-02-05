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
*/
void ignoreSignal(int signo) {
	// pass
	//TODO 
	printShout("ignore time\n");
	return;
}

/* 
* TODO 
*/
void killZombieChildren() {
	return;
}