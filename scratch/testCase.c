#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
	bool testBool = false;
	int testVal = 0;
	switch (testVal) {
	case 0: {
		testBool = true;
		printf("bool value in 0 case: %d\n", testBool);
		break; }
	case 1: { break; }
	default: { 
		printf("bool value in default case: %d\n", testBool);
		break; }
	}
	return EXIT_SUCCESS;
}