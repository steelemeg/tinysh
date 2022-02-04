
/*
* Prints a specified string out to the shell.
* Accepts a pointer to the target string.
* Be sure you flush out the output buffers each time you print!
* Helps ensure that printf is always follwed by fflush.
* Returns no values.
*/
void printShout(const char* printTarget) {
    printf("%s", printTarget);
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
        printShout(arrayInput[i]);
        printShout("\n");
    }
}