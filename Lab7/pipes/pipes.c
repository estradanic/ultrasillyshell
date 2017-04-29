#include "pipes.h"
#include "../tokenize/makeArgs.h"
#include "../redirect/redirect.h"
#include "../alias/alias.h"
#include "../linkedlist/linkedList.h"
#include "../linkedlist/listUtils.h"
#include "../redirect/redirect.h"

int inFD = 0;
int outFD = 1;

int containsPipe(char *s) {
	int tokens;
	if (s == NULL) {
		tokens = -1;
	}
	else
	{
		char * copy;
		char delimiters[]= "|";
		char * newStr = (char *) calloc(strlen(s) + 1, sizeof(char));
		strcpy(newStr, s);
		tokens = 0;
		char * token = strtok_r(newStr, delimiters, &copy);
		while (token != NULL) {
			tokens++;
			token =  strtok_r(NULL, delimiters, &copy);
		}
		tokens--;
		free(newStr);
		newStr = NULL;
	}
	return tokens;
}

void parseWholePipe(char * s, char ** leftPipe, char ** rightPipe) {
	if (s == NULL) {
		exit(-1);
	}
	else {
		char *copy;
		char delimiters[] = "|";
		char *newStr = (char *) calloc(strlen(s) + 1, sizeof(char));
		strcpy(newStr, s);
		char *tempStrTok = strtok_r(newStr, delimiters, &copy);
		*leftPipe = (char *) calloc(strlen(tempStrTok) + 1, sizeof(char));
		strcpy(*leftPipe, tempStrTok);
		tempStrTok = strtok_r(NULL, delimiters, &copy);
		*rightPipe = (char *) calloc(strlen(tempStrTok) + 1, sizeof(char));
		strcpy(*rightPipe, tempStrTok);
		free(newStr);
		newStr = NULL;
	}
}

char ** parsePrePipe(char *s, int * preCount) {
	char ** argv;
	if (s == NULL) {
		exit(-1);
	}
	else {
		char * copy;
		char delimiters[]= "|";
		char * newStr = (char *) calloc(strlen(s) + 1, sizeof(char));
		strcpy(newStr, s);
		char * tempStrTok = strtok_r(newStr, delimiters, &copy);
		char * tempStr = (char *) calloc(strlen(tempStrTok) + 1, sizeof(char));
		strcpy(tempStr, tempStrTok);
		*preCount = makeargs(tempStr, &argv);
		free(newStr);
		newStr = NULL;
		free(tempStr);
		newStr = NULL;
		return argv;
	}
}

char ** parsePostPipe(char *s, int * postCount){
	char ** argv;
	if (s == NULL) {
		exit(-1);
	}
	else {
		char * copy = NULL;
		char delimiters[]= "|";
		char * newStr = (char *) calloc(strlen(s) + 1, sizeof(char));
		strcpy(newStr, s);
		char * tempStrTok = strtok_r(newStr, delimiters, &copy);
		tempStrTok = strtok_r(NULL, delimiters, &copy);
		char * tempStr = (char *) calloc(strlen(tempStrTok) + 1, sizeof(char));
		strcpy(tempStr, tempStrTok);
		*postCount = makeargs(tempStr, &argv);
		free(newStr);
		newStr = NULL;
		free(tempStr);
		tempStr = NULL;
		return argv;
	}
}

void pipeIt(char ** prePipe, char ** postPipe, char ** inRedirect, char ** outRedirect, LinkedList * historyList, int histcount) {
	pid_t pid;
	int fd[2], res, status;
	pid = fork();
	if(pid != 0) {
		waitpid(pid, &status, 0);
	}
	else {
		if (*inRedirect != NULL)
			redirectIn(*inRedirect);
		if (*outRedirect != NULL)
			redirectOut(*outRedirect);
		res = pipe(fd);
		if(res < 0)
		{
			printf("Pipe Failure\n");
			exit(-1);
		}// end if
		pid = fork();
		if(pid != 0)
		{
			close(fd[1]);
			close(0);
			dup(fd[0]);
			close(fd[0]);
			if (strcmp(*postPipe, "history") == 0) {
				printList(historyList, printHistoryType, (historyList->size - histcount));
			}
			int result = execvp(postPipe[0], postPipe);
			if (result == -1) {
				exit(-1);
			}
		}// end if AKA parent
		else
		{
			close(fd[0]);
			close(1);
			dup(fd[1]);
			close(fd[1]);
			if (strcmp(*prePipe, "history") == 0) {
				printList(historyList, printHistoryType, (historyList->size - histcount));
			}
			int result = execvp(prePipe[0], prePipe);
			if (result == -1) {
				exit(-1);
			}
		}// end else AKA child
	}
}
