// Functions for executing specific commands

/* The exit command exits your shell. 
*  First of the three built-in shell commands that smallsh handles. 
*  Accepts no arguments. 
*  Kills any child processes or jobs.
*/
void execExit() {

}

/*
* Executes instruction with arguments in the shell. Accepts a command struct containing parametersn
*/
void execCommand(struct command* currCommand) {

    // Comments mean we should do nothing! 
    if (!currCommand->isCommentOrBlank) { printShout(currCommand->instruction); }

    return;
}