#include "makeArgs.h"

void clean(int argc, char **argv)
{
	int index;
	for (index = 0; index < argc; index++) {
		free(*(argv + index));
		*(argv + index) = NULL;
	}
	free(argv);
	argv = NULL;
}// end clean

int makeargs(char *s, char *** argv) {
	if (s == NULL) {
		return -1;
	}
	else {
		char *copy1, *copy2;
		char delimiters[] = " \t\n";
		char firstNewString[MAX];
		strcpy(firstNewString, s);
		char secondNewString[MAX];
		strcpy(secondNewString, s);
		int tokens = 0;
		char *token = strtok_r(firstNewString, delimiters, &copy1);
		while (token != NULL) {
			tokens++;
			token = strtok_r(NULL, delimiters, &copy1);
		}
		*(argv) = (char **) calloc(tokens + 1, sizeof(char *));
		char *tempToken = strtok_r(secondNewString, delimiters, &copy2);
		int index = 0;
		while (tempToken != NULL) {
			(*argv)[index] = (char *) calloc(strlen(tempToken) + 1, sizeof(char));
			strcpy((*argv)[index], tempToken);
			index++;
			tempToken = strtok_r(NULL, delimiters, &copy2);
		}
		(*argv)[tokens] = '\0';
		return tokens;
	}
}
