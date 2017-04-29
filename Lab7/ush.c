#include "./utils/myUtils.h"
#include "./utils/ushrcUtils.h"
#include "./history/history.h"
#include "./linkedlist/listUtils.h"
#include "./linkedlist/linkedList.h"
#include "./alias/alias.h"
#include "./pipes/pipes.h"
#include "./tokenize/makeArgs.h"
#include "./redirect/redirect.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>
#include <linux/limits.h>

#define MAX 500
#define TRUE 1
#define FALSE 0

void forkIt(char* inRedirect, char* outRedirect, char** argv){
    pid_t pid = fork();
    if(pid != 0){
        int status = 0;
        pid_t parent = waitpid(pid, &status, NULL);
    }
    else{
        if (inRedirect != NULL)
            redirectIn(inRedirect);
        if (outRedirect != NULL)
            redirectOut(outRedirect);
        int isValid = execvp(argv[0], argv);
        if(isValid == -1){
            exit(-1);
        }
        exit(42);
    }
}

int main()
{
	int argc, histcount, histfilecount;
	char **argv = NULL, s[MAX], temp[MAX];
	LinkedList * aliasList = linkedList();
	LinkedList * historyList = linkedList();
	char ** prePipe = NULL, ** postPipe = NULL;
	FILE * fp = NULL;
	alias * checkAliasArgs = NULL;
	int pathSet = FALSE;
	FILE * tempFile = NULL;
	char * path = NULL;
	char *inRedirect = NULL;
    char* startingDir = getenv("PWD");
	char *outRedirect = NULL;
	fp = fopen(".ushrc", "r");
	if (fp != NULL) {
		histcount = getHistFileCount(fp, s);
		histfilecount = getHistFileCount(fp, s);
		fgets(s, MAX, fp);
		while (strcmp(s, "\n") == 0 && !feof(fp))
			fgets(s, MAX, fp);
		strip(s);
		if (!feof(fp)) {
			argc = makealiasargs(s, &argv);
			while (!feof(fp)) {
				addLast(aliasList, buildNode_Type(buildAliasType_Args(argv)));
				fgets(s, MAX, fp);
				while (strcmp(s, "\n") == 0 && !feof(fp))
					fgets(s, MAX, fp);
				if (!feof(fp)) {
					strip(s);
					argc = makealiasargs(s, &argv);
				}
			}
		}
		fclose(fp);
	}
	else {
		histcount = 100;
		histfilecount = 1000;
        fp = fopen(".ushrc", "w+");
        fputs("HISTCOUNT=100\nHISTFILECOUNT=1000\n", fp);
        fclose(fp);
	}
	strcpy(s, "");
	fp = fopen(".ush_history", "rw");
	if (fp != NULL) {
		fgets(s, MAX, fp);
		while (!feof(fp)) {
			argc = makehistoryargs(s, &argv);
			addLast(historyList, buildNode_Type(buildHistoryType_Args(argc, argv)));
			incrementHistoryCount();
			fgets(s, MAX, fp);
		}
	}
	else {
		fp = fopen(".ush_history", "w");
	}
	fclose(fp);
  	printf("command?: ");
  	fgets(s, MAX, stdin);
  	strip(s);
  	while(strcmp(s, "exit") != 0) {
        if(strlen(s) != 0){
            argc = makehistoryargs(s, &argv);
            history * currentArgs = (history *)buildHistoryType_Args(argc, argv);
            Node * previousCommand= retrieveNthLast(historyList, 0);
            if(historyList->size == 0 || compareHistory(currentArgs, previousCommand->data) != 0) {
                addLast(historyList, buildNode_Type(currentArgs));
                incrementHistoryCount();
            }
            else {
                cleanTypeHistory(currentArgs);
            }
            if (s[0] == '!') {
                Node *previousCommand;
                int val;
                if (s[1] == '!') {
                    previousCommand = retrieveNthLast(historyList, 1);
                }
                else {
                    char * p = s;
                    p++;
                    strcpy(s, p);
                    val = (int) strtol(s, &p, 10);

                    previousCommand = retrieveNth(historyList, val);
                }
                history *lastCommand = previousCommand->data;
                strcpy(s, lastCommand->argv[0]);
                int index;
                for (index = 1; index < lastCommand->argc; index++) {
                    strcat(s, " ");
                    strcat(s, lastCommand->argv[index]);
                }
            }
            int pipes = containsPipe(s);
            if (pipes > 0) {
                char * prePipeString = NULL;
                char * postPipeString = NULL;
                char * preCommand, * postCommand;
                parseWholePipe(s, &preCommand, &postCommand);
                checkForRedirection(preCommand, &prePipeString, &inRedirect, &outRedirect);
                checkForRedirection(postCommand, &postPipeString, &inRedirect, &outRedirect);
                checkForAlias(&prePipeString, aliasList);
                checkForAlias(&postPipeString, aliasList);
                if (strstr(prePipeString, "alias ") == prePipeString) {
                    argc = makealiasargs(s, &argv);
                    addLast(aliasList, buildNode_Type(buildAliasType_Args(argv)));
                    argv = NULL;
                }
                if (strstr(postPipeString, "alias ") == postPipeString) {
                    argc = makealiasargs(s, &argv);
                    addLast(aliasList, buildNode_Type(buildAliasType_Args(argv)));
                    argv = NULL;
                }
                if (strstr(prePipeString, "unalias ") == prePipeString) {
                    printf("Can't get this to work without errors...\n");
                }
                char ** prePipe, ** postPipe;
                int leftCount = 0, rightCount = 0;
                leftCount = makeargs(prePipeString, &prePipe);
                rightCount = makeargs(postPipeString, &postPipe);
                if (strcmp(prePipe[0], "cd") == 0) {
                    if(chdir(prePipe[1])==-1){
                        printf("Directory not found\n");
                    }
                    else{
                        setenv("PWD", prePipe[1], 1);
                    }
                }
                if (strcmp(postPipe[0], "cd") == 0) {
                    if(chdir(postPipe[1])==-1){
                        printf("Directory not found\n");
                    }
                    else{
                        setenv("PWD", postPipe[1], 1);
                    }
                }
                pipeIt(prePipe, postPipe, &inRedirect, &outRedirect, historyList, histcount);
                free(prePipeString);
                prePipeString = NULL;
                free(postPipeString);
                postPipeString = NULL;
                free(preCommand);
                preCommand = NULL;
                free(postCommand);
                postCommand = NULL;
                clean(leftCount, prePipe);
                clean(rightCount, postPipe);
                if (inRedirect != NULL) {
                    free(inRedirect);
                    inRedirect = NULL;
                }
                if (outRedirect != NULL){
                    free(outRedirect);
                    outRedirect = NULL;
                }
            }
            else {
                char *command;
                checkForRedirection(s, &command, &inRedirect, &outRedirect);
                command = strstrip(command);
                checkForAlias(&command, aliasList);
                if (strstr(command, "alias ") == command) {
                    argc = makealiasargs(s, &argv);
                    addLast(aliasList, buildNode_Type(buildAliasType_Args(argv)));
                    argv = NULL;
                }
                if (strstr(command, "unalias ") == command) {
                    printf("Can't get this to work...\n");
                }
                argc = makeargs(command, &argv);
                if (strcmp(argv[0], "history") == 0) {
                    printList(historyList, printHistoryType, (historyList->size - histcount));
                }
                else if(strcmp(argv[0], "cd") == 0){
                    if(chdir(argv[1])==-1)
                        printf("Directory not found.\n");
                    else
                        setenv("PWD", argv[1], 1);
                }
                else{
                    forkIt(inRedirect, outRedirect, argv);
                }
                free(command);
                command = NULL;
                clean(argc, argv);
                argv = NULL;
                if (inRedirect != NULL) {
                    free(inRedirect);
                    inRedirect = NULL;
                }
                if (outRedirect != NULL){
                    free(outRedirect);
                    outRedirect = NULL;
                }
            }
        }
		printf("command?: ");
	  	fgets(s, MAX, stdin);
		strip(s);
  	}// end while
	char * historyPath = (char *)calloc(strlen(startingDir) + strlen("/.ush_history") + 1, sizeof(char));
	strcpy(historyPath, startingDir);
	strcat(historyPath, "/.ush_history");
	fp = fopen(historyPath, "w");
	free(historyPath);
	historyPath = NULL;
	if (fp != NULL) {
		ftruncate(fileno(fp), 0);
		rewind(fp);
		int skip = historyList->size - histfilecount;
		if (skip < 0)
			skip = 0;
		int index;
		for (index = 0; index < skip; index++) {
			removeFirst(historyList, cleanTypeHistory);
		}
		while (historyList->size > 0) {
			Node * curr = retrieveFirst(historyList);
			history * listing = (history *)curr->data;
			int index2;
			for (index2 = 0; index2 < listing->argc; index2++) {
				fputs(listing->argv[index2], fp);
				fputs(" ", fp);
			}
			fputs("\n", fp);
			removeFirst(historyList, cleanTypeHistory);
		}
		fclose(fp);
	}
	else {
		printf("Could not write history. Sorry :/\n");
	}
	free(path);
	path = NULL;
	clearList(historyList, cleanTypeHistory);
   	free(historyList);
   	historyList = NULL;
	clearList(aliasList, cleanTypeAlias);
   	free(aliasList);
   	aliasList = NULL;
    unsetenv("PWD");
    printf("Bye!\n");
  	return 0;
}// end main

