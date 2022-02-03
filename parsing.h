// Defining some items for frequent use.
#define DELIMITER " "
// Your shell must support command lines with a maximum length of 2048 characters, and a maximum of 512 arguments.
#define MAX_COMMAND 2048
#define MAX_ARG 512

// As recommended in the assignment, creating a struct to hold commands and their possible parameters.
struct command {
    char* instruction;
    bool isCommentOrBlank;
    int operandCount;
    char** operands;
};


/*
* Accepts user input, creates a new command struct based on its contents
* Accepts blanks and comments based on the spec, but flags them for later ignoring.
* Structure originally based on project 1 createMovie, draws heavily from strtok and strcpy usage there.
* Per the specs, quoting is not supported and arguments with spaces inside them are not possible.
* Therefore, using space as the delimiter. 
*/
struct command* createCommand(char* userInput) {
    char* saveptr;
    char* token;
    int argCount = 0;
    struct command* newCommand = malloc(sizeof(struct command));
    char* copyInput = calloc(strlen(userInput) + 1, sizeof(char));
    strcpy(copyInput, userInput);
    const char comment = '#';
    const char space = ' ';     

    // The first token should be the instruction. It should also tell us if this is a blank or comment.
    token = strtok_r(userInput, DELIMITER, &saveptr);

    // Start the argument counter at zero
    newCommand->operandCount = argCount;

    // Handle blank or commented inputs.   
    if (token == NULL || token[0] == comment) {
        newCommand->isCommentOrBlank = true;
        return newCommand;
    }
    else { newCommand->isCommentOrBlank = false; }
    
    // Inner calloc / strcpy based on Project 1, if statements protect from blanks and comments.
    // Originally ignored comments entirely, but this caused problems with printing the start-of-line colon. 
    newCommand->instruction = calloc(strlen(token) + 1, sizeof(char));
    strcpy(newCommand->instruction, token);

    // The output from getExpandedInput has been space-condensed. Therefore we can use spaces as a proxy 
    // for the number of arguments.
    printf("pre space count %d\n", argCount);
    for (int n = 0; copyInput[n]; n++) {
        printf("in loop\n");
        if (copyInput[n] == *DELIMITER) { argCount++; }
    }
    printf("space count %d\n", argCount);



    return newCommand;
}

/*
* Get input from the user.
* Check if it is a comment or a blank line.
* If not, per the project guide, expand any instance of "$$" in a command into the process ID of the smallsh itself.
* No other variable expansion is performed.
* Returns user input, expanded if appropriate, for parsing into a command.
*/
char* getExpandedInput() {
    // Get the user's input. Per the assignment, we know that 
    // commands will not be more than 2048 characters long.
    char* rawInput = calloc(MAX_COMMAND, sizeof(char));
    // using fgets instead of getline based on project 2 testing
    fgets(rawInput, MAX_COMMAND, stdin);
    // Drop the trailing newline
    rawInput[strcspn(rawInput, "\n")] = 0;

    // Screen for blanks and comments now
    const char comment = '#';
    if (rawInput[0] == '\0' || rawInput[0] == comment) {
        return rawInput;
    }

    // If the string has non-comment commands, start working on the expansion.
    size_t origSize = strlen(rawInput);
    char* doubleDollar = "$$";
    int expandedSize = 0;

    char* expandedOutput = calloc(origSize + 1, sizeof(char));
    pid_t pid = getpid();

    // The max possible PID is somewhere between 32768 and 2^22 per https://stackoverflow.com/questions/6294133/maximum-pid-in-linux 
    // Formal citation in readme.
    // Going with a max allowed of 11 to be on the safe side, adding 1 for null terminator
    char* currPid = calloc(12, sizeof(char));
    // Borrowing sprintf trick from Project 2
    sprintf(currPid, "%d", pid);
    size_t pidSize = strlen(currPid);

    // Iterate through the input string, copying characters into the expandedInput string. If $$s are encountered,
    // substitute the pid and realloc expanded input to ensure there is enough space. Based off of the strcpy variant 
    // from my Project 1.
    // I tried to handle odd numbers of $s (i.e. "input $$$") like the actual shell and just 
    // expanded the first two (i.e. resulting in "input 12345%"). 
    {
        // Use these as array pointers into the input and expandedOutput arrays
        int inputPtr = 0;
        int outputPtr = 0;
        const char singleDollar = '$';
        const char space = ' ';
        bool firstDollarFound = false;

        while (rawInput[inputPtr] != '\0') {
            // If the input isn't a $, copy it straight into the output 
            if (rawInput[inputPtr] != singleDollar) {
                // Just in case we get any inputs with multiple spaces, condense them to single spaces.
                if (rawInput[inputPtr] == space && rawInput[inputPtr + 1] == space) { inputPtr++; }
                else {
                    expandedOutput[outputPtr] = rawInput[inputPtr];
                    inputPtr++;
                    outputPtr++;
                }
            }
            // If a $ is located, is the next character also $? If not, just keep the $ and move on.
            else if (rawInput[inputPtr] == singleDollar && (rawInput[inputPtr + 1] != singleDollar)) {
                expandedOutput[outputPtr] = rawInput[inputPtr];
                inputPtr++;
                outputPtr++;
            }
            // If a $ is located and the next character is also $, expand the PID.
            else if (rawInput[inputPtr] == singleDollar && (rawInput[inputPtr + 1] == singleDollar)) {
                // Make sure there is enough room in the expanded output, since the actual PID will need more than 2 characters.
                // Don't need to add the full length of the PID, since the original calloc accounted for the two $$ characters.
                expandedSize = strlen(expandedOutput) + pidSize - 2;
                expandedOutput = realloc(expandedOutput, expandedSize); 

                // Append the pid to the expanded output
                strcat(expandedOutput, currPid);
                // Move both pointers. input pointer moves by 2, output pointer moves by pid size
                inputPtr = inputPtr + 2;
                outputPtr = outputPtr + pidSize;
            }            
        }
    }
    free(rawInput);
    return expandedOutput;
}