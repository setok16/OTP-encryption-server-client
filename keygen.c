#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	if (argc <= 1) {
		fprintf(stderr, "USAGE: %s length\n", argv[0]);
		return -1;
	}

	time_t t;
	srand((unsigned) time(&t)); // Initializing random number generator

	int arg = atoi(argv[1]); // Stores int representation of argument
	char* key = calloc(arg, sizeof(char));
	int count;
	int selection;

	for(count = 0; count < arg; count++) {
		selection = (rand() % 27 + 64); // Assigning ascii values from '@ to Z'
		if (selection == 64) selection = 32; // Converting '@' to ' '
		key[count] = selection;
	}
		
	printf("%s\n", key);

	return 0;
}
