/*
* TODO
*/
void redirector(char* targetFile) {
	char* filename;
	if (targetFile != NULL) { filename = targetFile; }
	else { filename = "/dev/null"; }
	printShout(filename);
}