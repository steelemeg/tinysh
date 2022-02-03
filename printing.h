
/*
* Prints a specified string out to the shell. SHOUT IT OUT.
* Be sure you flush out the output buffers each time you print!
* Helps ensure that printf is always follwed by fflush.
*/
void printShout(const char* printTarget) {
    printf("%s", printTarget);
    fflush(NULL);

    return;
}

/*
* Based on the screenshots, the terminal lines must start with a colon.
* Breaking this out as a special case.
*/
void printStartTerminal() {
    char* colon = ": ";
    printShout(colon);

    return;
}