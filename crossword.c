#include <stdio.h>
#include <stdlib.h>
#include "puz.h"

/**
 * A pointer to the Crossword object that is loaded at the start of main.
 * Here, "static" means that this variable can only be used in this file.
 * It was declared globally here for convenience. You can directly access 
 * this variable in any function inside this file.
 *
 * See the documentation (or puz.h) for the Crossword struct's members.
 */
static Crossword* cross = NULL;

/**
 * Semantically, this is a pointer to an array of Clue objects.	 You can also 
 * think of it as an array of pointers to Clue objects. The purpose of this 
 * variable is to "map" an int index (like 0) to a Clue (like "1 Across").
 * This is convenient, because you can access all the clue information such 
 * as its location on the board, its text, and how long its answer is.
 */
static Clue** clueMap = NULL;

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("USAGE: ./crossword /path/to/crossword.puz\n");
		exit(1);
	}

	cross = createCrossword(argv[1]);
	clueMap = createClueMap(cross);

	printf("Title: %s\n", cross->title);
	printf("Author: %s\n", cross->author);
	printf("Copyright: %s\n\n", cross->copyright);

	/* YOUR CODE HERE */

	destroyClueMap(cross, clueMap);
	destroyCrossword(cross);

	return EXIT_SUCCESS;
}
