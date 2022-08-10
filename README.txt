Limited feature set implementation of a bash shell. Built in support for exit, cd, and status; passes other commands to exec. Supports expansion, redirection, background commands, signal handling, and parent/child process forking. 

Run Instructions
	Run the following to compile:
		
		make clean
		make
		.smallsh
		
	If there are any makefile errors, compile manually using: 
	
		gcc --std=gnu99 -o smallsh smallsh.c

	

		
