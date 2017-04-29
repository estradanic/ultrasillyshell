#ifndef ALIAS_H
#define ALIAS_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Alias
{
	char ** argv;
} alias;

int makealiasargs(char *s, char *** argv);

void * buildAliasType_Args(char **argv);

int compareAlias(const void * p1, const void * p2);

void cleanTypeAlias(void * passedIn);

#endif
