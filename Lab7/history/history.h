#ifndef HISTORY_H
#define HISTORY_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 500

typedef struct History
{
    int argc;
    char ** argv;
    int number;
} history;

void incrementHistoryCount();

int makehistoryargs(char *s, char *** argv);

void * buildHistoryType_Args(int argc, char **argv);

void printHistoryType(void * passedIn);

int compareHistory(const void * p1, const void * p2);

void cleanTypeHistory(void * passedIn);

#endif
