// Defining some items for frequent use.
#define DELIMITER " "
// Your shell must support command lines with a maximum length of 2048 characters, and a maximum of 512 arguments.
#define MAX_COMMAND 2048
#define MAX_ARG 512

// As recommended in the assignment, creating a struct to hold commands and their possible parameters.
struct command {
    char* instruction;
    int operandCount;
    char** operands;
    bool isCommentOrBlank;
};

/*
* Accepts user input, creates a new command struct based on its contents
* Accepts blanks and comments based on the spec, but flags them for later ignoring.
* Structure originally based on project 1 createMovie, draws heavily from strtok and strcpy usage there.
*/
struct command* createCommand(char* userInput) {
    char* saveptr;
    char* token;
    struct command* newCommand = malloc(sizeof(struct command));
    char* copyInput = calloc(strlen(userInput) + 1, sizeof(char));
    strcpy(copyInput, userInput);
    const char comment = '#';

    // The first token should be the instruction. It should also tell us if this is a blank or comment.
    token = strtok_r(userInput, DELIMITER, &saveptr);
    // Handle bad blank inputs.   
    if (token == NULL) {
        newCommand->isCommentOrBlank = true;
        return newCommand;
    }
    
    // Inner calloc / strcpy lifted from Project 1, if statements protect from blanksand comments.
        // Originally ignored comments entirely, but this caused problems with printing the start-of-line colon. 
        newCommand->instruction = calloc(strlen(token) + 1, sizeof(char));
        strcpy(newCommand->instruction, token);
        
  
    // Moving comment handling logic -- comments still create commands, but they'll be flagged appropriately.
    if (newCommand->instruction[0] == comment) {
        newCommand->isCommentOrBlank = true;
    }
    else { newCommand->isCommentOrBlank = false; }
    return newCommand;
}

/*
* Expansion was hitting issues because strtok_r only accepted one delimiter, not a delimiter string.
* Rework of strtok_r to allow string delimiter.
*/
char* strtokStr(char* input, char* delimiter, char** savePtr, char* currPid)
{
    char* isDelimPresent;
    if (input == NULL)
        input = *savePtr;
    // Screen for the end of the string
    if (input == NULL || *input == '\0')
    {
        *savePtr = input;
        return NULL;
    }

    // First, check if there are even any delimiters present.
    // Use strstr to search for the delimiter substring.
    isDelimPresent = strstr(input, delimiter);
    // If the delimiter was never found, the input was the last token. Move the pointer to the end.
    if (end == NULL)
    {
        *savePtr = input + strlen(input);
        return input;
    }                       
    return strcat(input, currPid);
}


/*
* Get input from the user.
* Check if it is a comment or a blank line.
* If not, per the project guide, expand any instance of "$$" in a command into the process ID of the smallsh itself.
* No other variable expansion is performed.
* Returns appropriate input for parsing into a command.
*/
char* getExpandedInput() {
    // Get the user's input. Per the assignment, we know that 
    // commands will not be more than 2048 characters long.
    char* input = calloc(MAX_COMMAND, sizeof(char));
    // using fgets instead of getline based on project 2 testing
    fgets(input, MAX_COMMAND, stdin);
    // Drop the trailing newline
    input[strcspn(input, "\n")] = 0;
  
    // Screen for blanks and comments
    const char comment = '#';
    if (input[0] == '\0' || input[0] == comment) {
        return input;
    }
    
// If the string has non-comment commands, start working on the expansion.
size_t origSize = strlen(input);
char* doubleDollar = "$$";
int expandedSize = 0;
int n = 0;
int partialSize = 0;
char* copyInput = calloc(origSize + 1, sizeof(char));
char* saveptr;
char* token;
bool trailing = false;
char* expandedInput = calloc(origSize + 1, sizeof(char));
pid_t pid = getpid();

// The max possible PID is somewhere between 32768 and 2^22 per https://stackoverflow.com/questions/6294133/maximum-pid-in-linux 
// Formal citation in readme.
// Going with a max allowed of 11 to be on the safe side, adding 1 for null terminator
char* currPid = calloc(12, sizeof(char));
// Borrowing sprintf trick from Project 2
sprintf(currPid, "%d", pid);

token = strtokStr(input, doubleDollar, &saveptr, currPid);
partialSize = strlen(token);

// So if the length of the raw input is the same as token, then $$ never appeared and processing is done.
if (origSize == partialSize) {
    strcpy(expandedInput, input);
    free(copyInput);
    free(currPid);
    return expandedInput;
}
printf("pid: %s\n", currPid);
// I don't know how many tokens or how many $$s are in the input. Loop through the input until the end, building the expanded input string.
// Basing logic off of the languages substruct from my Project 2, similar issue of needing to strtok until the token is empty
// This has the potential to go over the max char limit after expansion. Upping the size as we go.
// Flow: Token holds the input up until $$. Realloc the output. Concat the token the the output, concat the pid to the output.
//      Get a new token. Check if this is the end or if there are trailing chars.
while (true) {
    expandedSize = strlen(expandedInput) + strlen(token) + strlen(currPid);
    if (expandedSize > (origSize + 1)) { expandedInput = realloc(expandedInput, expandedSize); }

    // Tack on the token and, if relevant, the PID
    strcat(expandedInput, token);
    // Was having an issue where the PID got appended to all PID-containing inputs.
    // Fix: Check to see if the remaining str contains any $$ before appending.
    //if (strstr(saveptr, doubleDollar)) {
    //strcat(expandedInput, currPid);
    //}
    printf("remaining to process %s len %d\n", saveptr, strlen(saveptr));
    printf("expansion %s\n", expandedInput);
    token = strtokStr(NULL, doubleDollar, &saveptr, currPid);
}

free(currPid);
free(copyInput);
printf("final output %s\n", expandedInput);
return expandedInput;

}

char* getExpandedInput2() {
    // Get the user's input. Per the assignment, we know that 
    // commands will not be more than 2048 characters long.
    char* rawInput = calloc(MAX_COMMAND, sizeof(char));
    // using fgets instead of getline based on project 2 testing
    fgets(rawInput, MAX_COMMAND, stdin);
    // Drop the trailing newline
    rawInput[strcspn(rawInput, "\n")] = 0;

    // Screen for blanks and comments
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
    {
        // Use these as array pointers into the input and expandedOutput arrays
        int inputPtr = 0;
        int outputPtr = 0;
        const char singleDollar = '$';
        bool firstDollarFound = false;

        while (rawInput[inputPtr] != '\0') {
            // If the input isn't a $, copy it straight into the output 
            if (rawInput[inputPtr] != singleDollar) {
                expandedOutput[outputPtr] = rawInput[inputPtr];
                inputPtr++;
                outputPtr++;
            }
            // If a $ is located, is the next character also $? If not, just keep the $ and move on.
            else if (rawInput[inputPtr] == singleDollar && (rawInput[inputPtr + 1] != singleDollar)) {
                expandedOutput[outputPtr] = rawInput[inputPtr];
                inputPtr++;
                outputPtr++;
            }
            // If a $ is located and the next character is also $, expand the PID.
            else if (rawInput[inputPtr] == singleDollar && (rawInput[inputPtr + 1] == singleDollar)) {
                // Make sure there is enough room in the expanded output
                // Don't need to add the full length of the PID, since the original alloc accounted for the two $$ characters.
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
    printf("here's what we got %s\n", expandedOutput);
    return expandedOutput;

}