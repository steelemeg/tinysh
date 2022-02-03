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
*  Per the project guide, expand any instance of "$$" in a command into the process ID of the smallsh itself.
* No other variable expansion is performed.
* Takes the raw user input, returns expanded input.
*/
char* expansion(char* rawInput) {
    int rawSize = strlen(rawInput);
    char* doubleDollar = "$$";    
    int expandedSize = 0;
    int partialSize = 0;
    char* copyInput = calloc(strlen(rawInput) + 1, sizeof(char));
    char* saveptr;
    char* token;
    char* buffer = calloc(strlen(rawInput) + 1, sizeof(char));
    char* expandedInput = calloc(strlen(rawInput) + 1, sizeof(char));

    // I don't know how many $$s will be present but I can get a rough idea by counting
    // Allows memory allocation for expanding the input 
    // TODO temporarily removing this in favor of a memory shuffle method -- strcat got very messy.
    /*int i, numDoll;
    *for (i = 0, numDoll = 0; rawInput[i]; i++)
    *    numDoll += (rawInput[i] == '.');
    */

    token = strtok_r(rawInput, doubleDollar, &saveptr);
    partialSize = strlen(token);

    // So if the length of the raw input is the same as token, then $$ never appeared.
    if (rawSize == partialSize) {
        strcpy(expandedInput, rawInput);
        return expandedInput;
    }

    // Otherwise, we need to plug in the PID for all instances of $$, and I'm assuming there can be more than once.
    // The max possible PID is somewhere between 32768 and 2^22 per https://stackoverflow.com/questions/6294133/maximum-pid-in-linux 
    // Formal citation in readme.
    // Going with a max allowed of 11 to be on the safe side, adding 1 for null terminator
    
    char* currPid = calloc(12, sizeof(char));
    pid_t pid = getpid();
    // Borrowing sprintf trick from Project 2
    sprintf(currPid, "%d", pid);

    // I don't know how many tokens or how many $$s are in the input. Loop through the input until the end, building the expanded input string.
    // Basing logic off of the languages substruct from my Project 2, similar issue of needing to strtok until the token is empty
    // This has the potential to go over the max char limit after expansion. Upping the size as we go.
    // Flow: Token holds the input up until $$. Put token into buffer, put pid into buffer. Resize buffer. Get next token.
    while (token[0] != '\0') {
        // Time for a sliding tile puzzle.  Resize the buffer. Stash the expansion in the buffer, free the expansion,
        // resize the expansion, then bring it back. There has got to be a better way to do this.
        // wait -- realloc?
        expandedSize = strlen(expandedInput);
        realloc(expandedInput, expandedSize + strlen(currPid));
        strcat(expandedInput, currPid);
        token = strtok_r(rawInput, doubleDollar, &saveptr);        
    }

    return expandedInput;

    
    


}