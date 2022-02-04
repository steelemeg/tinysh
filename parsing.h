
// As recommended in the assignment, creating a struct to hold commands and their possible parameters.
struct command {
    char* instruction;
    bool isCommentOrBlank;
    int operandCount;
    char** operands;
};

// Opting to track child processes in a linked list.
struct child {
    int childPid;
    struct child* next;
};

/*
*  Creates a new child struct.
*  Accepts one input, the integer pid.
*  Returns the new struct.
*/
void createChild(struct child** first, int pid) {
    struct child* newChild = (struct child*)malloc(sizeof(struct child));
    newChild->next = (*first);
    newChild->childPid = pid;
    (*first) = newChild;   
}

/*
*  Removes a child struct from the linked list.
*  Accepts one input, the integer pid.
*  Returns a bool indicating success or failure of the removal.
*/
bool removeChild(struct child** first, int pid) {
    // Track the previous and next nodes to snip out the target
    struct child* currChild = *first, *prevChild;
    bool found = false;

    if (currChild != NULL && currChild->childPid == pid) {
        *first = currChild->next;
        found = true;
        free(currChild);
        return found;
    }
    while (currChild != NULL && currChild->childPid != pid)
    {
        prevChild = currChild;
        currChild = currChild->next;
    }
    if (currChild == NULL) { found = false; }
    
    // Reroute the previous node linkage to the next node 
    else {
        prevChild->next = currChild->next;
        found = true;
    }

    free(currChild);
    return found;
}

/*
* Print the linked list of all child processes. Included for testing purposes. Copied from Assignment 1.
* Accepts a child node that is used as the head.
* Returns no values.
*/
void childList(struct child* currChild)
{
    while (currChild != NULL)
    {
        printf("child id %d\n", currChild->childPid);
        currChild = currChild->next;
    }
}


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
    int operandCounter = 0;
    struct command* newCommand = malloc(sizeof(struct command));
    char* copyInput = calloc(strlen(userInput) + 1, sizeof(char));
    strcpy(copyInput, userInput);
    const char comment = '#';
    bool redirection = false;

    int tokenLength = 0;
    // Set up a blank array to hold the inputs. We know there will be a maximum of 512 arguments.
    newCommand->operands = calloc(MAX_ARG, sizeof(char*));

    // The first token should be the instruction. It should also tell us if this is a blank or comment.
    token = strtok_r(userInput, DELIMITER, &saveptr);

    // Start the argument counter at zero
    newCommand->operandCount = operandCounter;

    // Handle blank or commented inputs.   
    if (token == NULL || token[0] == comment) {
        newCommand->isCommentOrBlank = true;
        return newCommand;
    }
    else { newCommand->isCommentOrBlank = false; }
    
    // Inner calloc / strcpy based on Project 1, if statements protect from blanks and comments.
    // Originally ignored comments entirely, but this caused problems with printing the start-of-line colon reliably. 
    newCommand->instruction = calloc(strlen(token) + 1, sizeof(char));
    strcpy(newCommand->instruction, token);

    // The output from getExpandedInput has been space-condensed. Therefore we can use spaces as a proxy 
    // for the number of arguments. Idea from https://www.geeksforgeeks.org/program-count-occurrence-given-character-string ,
    // formal citation in the readme
    for (int n = 0; copyInput[n]; n++) {
        if (copyInput[n] == *DELIMITER) { operandCounter++; }
    }
    newCommand->operandCount += operandCounter;

    // Attempt to grab the next token if there is one, then keep going until the end of the input.
    token = strtok_r(NULL, DELIMITER, &saveptr);
    while (token != NULL) {
        tokenLength = strlen(token);
        // Look for special characters that indicate &, redirection, or adjacent commands
        if (token[0] == *AMPERSAND)
        if (token[0] == *LEFT_ARROW || token[0] == *RIGHT_ARROW) { redirection = true; }
        token = strtok_r(NULL, DELIMITER, &saveptr);
    }
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
            // if there is a leading space, strip it
            if (outputPtr == 0 && rawInput[inputPtr] == space) { inputPtr++; }
            
            // If the input isn't a $, copy it straight into the output 
            else if (rawInput[inputPtr] != singleDollar) {
                // Just in case we get any inputs with multiple spaces, condense them to single spaces.
                if (rawInput[inputPtr] == space && rawInput[inputPtr + 1] == space) { inputPtr++; }
                // if there is a trailing space, strip it
                else if (rawInput[inputPtr] == space && rawInput[inputPtr + 1] == '\0') { inputPtr++; }
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