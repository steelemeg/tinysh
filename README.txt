
Run Instructions
	Locate the program files on os1. Run the following to compile:
		
		gcc --std=gnu99 -o smallsh smallsh.c

	Run the following to execute with the test script:
		./p3testscript 2>&1 | more

	Run the following to execute manually if needed:
		./smallsh

		

Notes for my grader: 
	I noticed that the test script did not show all line breaks and sometimes had some weird formatting. Per TA advice in office hours, 
	this seems to be normal.

	I tried to develop in logical modules to make work a little easier, and to avoid the massive file I had by the end of Assignment 2.
	If there is a better way to do this I would love any pointers!

	Please compile using the option described above. (I have included a makefile in case it is required but noticed some odd signalling interactions when using it. Please use the gcc options above!)

Sources Used
Brewster, Benjamin (January 31, 2022) 3.1 Process [Video lecture] https://www.youtube.com/watch?v=1R9h-H2UnLs

Reddit [January 30, 2022] 344 smallsh project [Advice on breaking down the program into workable problems; see Ok_Park4538's post] https://www.reddit.com/r/OSUOnlineCS/comments/lb0jg2/344_smallsh_project/

Stack Overflow(January 24, 2022) Removing trailing newline character from fgets() input[Example source code] https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input

Stack Overflow (February 2, 2022) Maximum PID in Linux [Documentation] https://stackoverflow.com/questions/6294133/maximum-pid-in-linux 

OSU CS344 (February 2, 2022) Exploration: Shell Commands Related to Processes [Example source code] https://canvas.oregonstate.edu/courses/1884946/pages/exploration-shell-commands-related-to-processes

OSU CS344 (February 3, 2022) Exploration: Processes and I/O [Example source code] https://canvas.oregonstate.edu/courses/1884946/pages/exploration-processes-and-i-slash-o?

Klitzke, Evan (February 2, 2022) Path MAX is tricky [Documentation] https://eklitzke.org/path-max-is-tricky

OSU CS344 (February 2, 2022) Exploration: Signal Handling (Example Code Handlers for SIGINT, SIGUSR2, and Ignoring SIGTERM, etc. [Example Source Code] https://canvas.oregonstate.edu/courses/1884946/pages/exploration-signal-handling-api

Malmstein, Robert (February 2, 2022) This is the Story of a Shell [Theory and example source code] https://medium.com/@romalms10/this-is-the-story-of-a-shell-73686729a650

Ed Discussion (February 1, 2022) Exit Function [Assignment clarification] https://edstem.org/us/courses/16718/discussion/1064594

Tutorials Point (February 1, 2022) C Library function - exit() [Example source code] https://www.tutorialspoint.com/c_standard_library/c_function_exit.htm

Geeks for Geeks (February 2, 2022) Count Occurence in Given String [Example source code] https://www.geeksforgeeks.org/program-count-occurrence-given-character-string/

Stack Overflow (February 2, 2022) Chdir to Home Directory [Example source code] https://stackoverflow.com/questions/9493234/chdir-to-home-directory

TODO text citations page 363-364 The Current Working Directory of a Process

Delft Stack [February 2, 2022] C Initialize Array to 0 [Example source code] https://www.delftstack.com/howto/c/c-initialize-array-to-0/

Geeks for Geeks [February 2, 2022] Linked List Set 3 Deleting Node [Example source code] https://www.geeksforgeeks.org/linked-list-set-3-deleting-node/

Code Vault [February 1, 2022] Redirecting Standard Output in C [Example source code] https://www.youtube.com/watch?v=5fnVr-zH-SE

Delft Stack (February 1, 2022) Kill Child Process in C [Example source code] https://www.delftstack.com/howto/c/kill-child-process-in-c/

OSU CS344 (February 1, 2022) Module 5 Exploration:Process API - Creating and Terminating Process [Example source code] https://canvas.oregonstate.edu/courses/1884946/pages/exploration-process-api-creating-and-terminating-processes?module_item_id=21835972

Stack Overflow (February 3, 2022) How to Use Execvp [Example source code] https://stackoverflow.com/questions/27541910/how-to-use-execvp

Ruttan, Arden (Febraury 5, 2022) Introduction To Unix Signals Programming [Example source code] http://www.cs.kent.edu/~ruttan/sysprog/lectures/signals.html

CS344 ED Discussion (February 5, 2022) sigaction struct initialization [Example source code] https://edstem.org/us/courses/16718/discussion/1077481

Linux die.net (February 4, 2022) waitpid man page [Documentation] https://linux.die.net/man/2/waitpid

Stack Overflow (February 6, 2022) Proper Way to Free Memory of a Returned Variable [Example source code] https://stackoverflow.com/questions/29655051/proper-way-to-free-memory-of-a-returned-variable