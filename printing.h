
/*
* Prints a specified string out to the shell.
* Be sure you flush out the output buffers each time you print!
* Helps ensure that printf is always follwed by fflush.
*/
void printShout(const char* printTarget) {
    printf("%s", printTarget);
    fflush(NULL);

    return;
}