#ifndef PIPES_H
#define PIPES_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "../linkedlist/linkedList.h"
#include "../redirect/redirect.h"

int containsPipe(char *s);
void parseWholePipe(char * s, char ** leftPipe, char ** rightPipe);
char ** parsePrePipe(char *s, int * preCount);
char ** parsePostPipe(char *s, int * postCount);
void pipeIt(char ** prePipe, char ** postPipe, char ** inRedirect, char ** outRedirect, LinkedList * historyList, int histcount);

#endif 

