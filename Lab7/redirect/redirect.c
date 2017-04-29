#include "redirect.h"

void redirectIn(char * newInPath){
	freopen(newInPath, "r", stdin);
}

void redirectOut(char * newOutPath) {
	freopen(newOutPath, "w", stdout);
}
