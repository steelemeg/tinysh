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
    bool isComment;
};

/*
* Accepts user input, creates a new command struct based on its contents
* Structure originally based on project 1 createMovie.
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
    // Handle bad inputs. Inner calloc/strcpy lifted from Project 1, if statements protect from blanks and comments.
    if (token != NULL) {
        // Originally ignored comments entirely, but this caused problems with printing the start-of-line colon. 
        newCommand->instruction = calloc(strlen(token) + 1, sizeof(char));
        strcpy(newCommand->instruction, token);
        
    }
    // Moving comment handling logic -- comments still create commands, but they'll be flagged appropriately.
    if (newCommand->instruction[0] == comment) {
        newCommand->isComment = true;
    }
    else { newCommand->isComment = false; }
    return newCommand;
}

/* Get raw user input */
char* getRawInput() {
    char* input = calloc(MAX_COMMAND, sizeof(char));
    // using fgets instead of getline based on project 2 testing
    fgets(input, MAX_COMMAND, stdin);
    // Drop the trailing newline
    input[strcspn(input, "\n")] = 0;
    // TODO something with expansion, pain, etc
    return input;
}