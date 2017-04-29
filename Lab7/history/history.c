#include "history.h"

static int count = 1; //stupid hack

void incrementHistoryCount() {
	count++;
}

int makehistoryargs(char *s, char *** argv) {
	if (s == NULL) {
		return -1;
	}
	else {
		char * copy;
		char delimiters[]= " \t\n";
		char firstNewString[MAX];
		strcpy(firstNewString, s);
		char secondNewString[MAX];
		strcpy(secondNewString, s);
		int tokens = 0;
		char * token = strtok_r(firstNewString, delimiters, &copy);
		while (token != NULL) {
			tokens++;
			token =  strtok_r(NULL, delimiters, &copy);
		}
		*(argv) = (char **)calloc(tokens + 1, sizeof(char *));
		char * tempToken = strtok_r(secondNewString, delimiters, &copy);
		int index;
		for (index = 0; index < tokens; index++) {
			(*argv)[index] = (char *)calloc(strlen(tempToken) + 1, sizeof(char));
			strcpy((*argv)[index], tempToken);
			tempToken = strtok_r(NULL, delimiters, &copy);
		}
		argv[0][tokens] = '\0';
		return tokens;
	}
}


void * buildHistoryType_Args(int argc, char **argv) {
	history * data = (history *)calloc(1,sizeof(history));
	data->argc = argc;
	data->argv = argv;
	data->number = count;
	return data;
}

void printHistoryType(void * passedIn) {
	history * data = (history *)passedIn;
	printf("%d ", data->number);
	int index;
	for(index = 0; index < data->argc; index++)
		printf("%s ", data->argv[index]);
	printf("\n");
}


int compareHistory(const void * p1, const void * p2) {
	history * h1 = (history *)p1;
	history * h2 = (history *)p2;
	if (h1->argc > h2->argc)
		return -1;
	else if (h1->argc < h2->argc)
		return 1;
	else {
		int index;
		for (index = 0; index < h1->argc; index++) {
			if (strcmp(h1->argv[index], h2->argv[index]) != 0)
				return strcmp(h1->argv[index], h2->argv[index]);
		}
		return 0;
	}
}

void cleanTypeHistory(void * passedIn) {
	history * data = (history *)passedIn;
	int index;
	for (index = 0; index <= data->argc; index++) {
		free(data->argv[index]);
		data->argv[index] = NULL;
	}
	free(data->argv);
	data->argv = NULL;
	free(data);
	data = NULL;
}

