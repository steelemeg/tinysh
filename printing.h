
/*
* Prints a specified string out to the shell.
* Accepts a pointer to the target string and a boolean indicating if a newline is intended.
* Be sure you flush out the output buffers each time you print!
* Helps ensure that printf is always follwed by fflush.
* Returns no values.
*/
void printShout(const char* printTarget, bool lineBreak) {
    printf("%s", printTarget);
    if (lineBreak) { printf("\n"); }
    fflush(NULL);

    return;
}

/* 
* Specialized printing for error handling.
* Accepts a pointer to the target string.
* Prints it and fflush. 
* Returns no values.
*/
void printError(const char* printTarget) {
    perror(printTarget);
    fflush(NULL);
    return;
}

/*
* Prints the values in a specifed array to the shell.
* Accepts a pointer to the start of the array and an int specifying how many entries to print.
* Returns no values.
*/
void printArray(char* arrayInput[], int entries) {
    for (int i = 0; i < entries; i++) {
        printShout(arrayInput[i], true);
    }
}

/* 
* Used for debugging.
* Displays key metrics about the program.
* Accepts no parameters.
* returns no values.
*/
void exploreValues() {
    printf("Allow background mode: %d\n", allowBackgroundMode);
}