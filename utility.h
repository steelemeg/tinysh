// Contains functions for signal handling, redirection, and anyting else that supports other function types.

/*
* Use dup2 to redirect input or output.
* Accepts three parameters, the name of the file, a bool indicating if this is an input redirect, and a bool indicating 
* if this is an output redirect.
* Returns no values.
* Based on code from https://canvas.oregonstate.edu/courses/1884946/pages/exploration-processes-and-i-slash-o, full citation in readme
*/
void redirector(char* targetFile, bool input, bool output) {
	// Modifying lecture code to account for the possibility of a blank filename. 
	// Per assignment if filename is blank, use /dev/null
	char* filename;
	int fileD = 0;
	int result = 0;
	if (input && output) {
		printError("Invalid use of function redirector");
		return;
	}
	if (targetFile != NULL) { filename = targetFile; }
	else {
		filename = "/dev/null";
		printShout("NULL CHECK!");
	}
	printShout(filename);
	// If this is input, the file should already exist. If it's output, it might not.
	if (input) { fileD = open(filename, O_RDONLY); }
	else if (output) { fileD = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644); }

	// Handle file opening errors
	if (fileD == -1) { 
		printError("Problem opening file: "); 
		exit(1);
	}

	// If all is well, do the actual redirects
	if (input) { 
		result = dup2(fileD, 0);
	}
	else if (output) {
		result = dup2(fileD, 1);
	}
	// Handle dup2 errors 
	if (result == -1) {
		printError("Problem with redirection, dup2() execution generated an error");
		exit(2);
	}
	return;
}