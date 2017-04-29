#ifndef REDIRECT_H
#define REDIRECT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "../tokenize/makeArgs.h"
#include <fcntl.h>
#include <sys/stat.h>

void redirectIn(char * newInPath);
void redirectOut(char * newOutPath);

#endif 

