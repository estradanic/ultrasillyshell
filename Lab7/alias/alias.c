#include "alias.h"

int makealiasargs(char *s, char *** argv) {
	if (s == NULL) {
        perror("s can't be null");
		return -1;
	}
	else {
		char * copy;
		char delimiters[]= "=\"";
		char * newString = (char *) calloc(strlen(s) + 1, sizeof(char));
		strcpy(newString, s);
		int tokens = 2;
		*(argv) = (char **)calloc(tokens + 1, sizeof(char *));
		char * tempToken = strtok_r(s, delimiters, &copy);
		int x;
		for (x = 0; x < tokens; x++) {
			if (x == 0 && tempToken != NULL) {
				(*argv)[x] = (char *)calloc(strlen(tempToken) + 1 - 6, sizeof(char));
				strcpy((*argv)[x], &tempToken[6]);
			}
			else {
				(*argv)[x] = (char *) calloc(strlen(tempToken) + 1, sizeof(char));
				strcpy((*argv)[x], tempToken);
			}
			tempToken = strtok_r(NULL, delimiters, &copy);
		}
		argv[0][tokens] = '\0';
		free(newString);
		newString = NULL;
		return tokens;
	}
}

void * buildAliasType_Args(char **argv) {
	alias * data = (alias *)calloc(1,sizeof(alias));
	data->argv = argv;
	return data;
}

int compareAlias(const void * p1, const void * p2) {
	alias * one = (alias *)p1;
	alias * two = (alias *)p2;
	return strcmp(one->argv[0], two->argv[0]);
}

void cleanTypeAlias(void * passedIn) {
	alias * data = (alias *)passedIn;
	int x;
	for (x = 0; x <= 2; x++) {
		free(data->argv[x]);
		data->argv[x] = NULL;
	}
	free(data->argv);
	data->argv = NULL;
	free(data);
	data = NULL;
}
