#include "ushrcUtils.h"
#include "myUtils.h"

int getHistCount(FILE * fp, char * s) {
	fgets(s, MAX, fp);
	strip(s);
	char delimiters[]= "=";
	char * tempToken = strtok(s, delimiters);
	int num = atoi(s);
	return num;
}

int getHistFileCount(FILE * fp, char * s) {
	char * copy;
	fgets(s, MAX, fp);
	strip(s);
	char delimiters[]= "=";
	char * tempToken = strtok_r(s, delimiters, &copy);
	int num = atoi(copy);
	return num;
}
