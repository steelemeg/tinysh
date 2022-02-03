// Functions for executing specific commands

/*
* Executes instruction with arguments in the shell. Accepts a command struct containing parametersn
*/
void execCommand(struct command* currCommand) {

    // Comments mean we should do nothing! 
    if (currCommand->isCommentOrBlank) {
        //pass
        printShout("I'm a comment aaaaa");

    }
    // actual commands
    else { printShout(currCommand->instruction); }


    return;
}