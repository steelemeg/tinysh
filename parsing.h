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
char* getExpandedInput() {
    // Get the user's input. Per the assignment, we know that 
    // commands will not be more than 2048 characters long.
    char* input = calloc(MAX_COMMAND, sizeof(char));
    // using fgets instead of getline based on project 2 testing
    fgets(input, MAX_COMMAND, stdin);
    // Drop the trailing newline
    input[strcspn(input, "\n")] = 0;
    printf("%d\n", strlen(input));


    
    


}