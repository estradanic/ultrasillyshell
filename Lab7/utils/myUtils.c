#include "myUtils.h"
#include "../linkedlist/listUtils.h"

void strip(char *array) {
    if (array == NULL) {
        perror("array is null");
        exit(-99);
    }// end if
    int len = strlen(array), index = 0;
    while (array[index] != '\0' && index < len) {
        if (array[index] == '\r')
            array[index] = '\0';

        else if (array[index] == '\n')
            array[index] = '\0';

        index++;
    }// end while
}

char *strstrip(char *s){ // from the Linux Kernel
    size_t size;
    char *end;
    size = strlen(s);
    if (!size)
        return s;
    end = s + size - 1;
    while (end >= s && isspace(*end))
        end--;
    *(end + 1) = '\0';
    while (*s && isspace(*s))
        s++;
    return s;
}

void checkForBangs (char ** command, LinkedList * historyList) {
    Node *lastHistory;
    if (*(*command + 1) == '!') {
        lastHistory = retrieveNthLast(historyList, 1);
    }
    else {
        int val = 1;
        char * p = *command;
        while(*p) {
            if (isdigit(*p))
                val = (int)strtol(p, &p, 10);
            else
                p++;
        }

        lastHistory = retrieveNth(historyList, val);
    }
    history *lastCommand = lastHistory->data;
    char tempCommand[MAX];
    strcpy(tempCommand, lastCommand->argv[0]);
    int index;
    for (index = 1; index < lastCommand->argc; index++) {
        strcat(tempCommand, " ");
        strcat(tempCommand, lastCommand->argv[index]);
    }
    free(*command);
    *command = NULL;
    *command = (char *)calloc(strlen(tempCommand) + 1, sizeof(char));
    strcpy(*command, tempCommand);
}

void checkForAlias(char ** command, LinkedList * aliasList) {
    Node * current = aliasList->head->next;
    while (current != NULL) {
        alias * currAlias = current->data;
        if (strcmp(*command, currAlias->argv[0]) == 0) {
            free(*command);
            *command = NULL;

            *command = (char *)calloc(strlen(currAlias->argv[1]) + 1, sizeof(char));
            strcpy(*command, currAlias->argv[1]);
            break;
        }
        current = current->next;
    }
}

void checkForAliasToRemove(char ** argv, LinkedList * aliasList) {
//TODO
}

void checkForRedirection(char * s, char ** command, char ** redirectInPath, char ** redirectOutPath) {
    if (s == NULL) {
        exit(-1);
    }
    else {
        char * inRedirect = strstr(s, "<");
        char * outRedirect = strstr(s, ">");
        char * newString = (char *) calloc(strlen(s) + 1, sizeof(char));
        strcpy(newString, s);
        s = strstrip(s);
        char * copy = NULL;
        if (strstr(s, "<") != NULL && strstr(s, ">") != NULL) {
            if (strstr(inRedirect, ">") != NULL) {
                char * tempToken = strstrip(strtok_r(newString, "<", &copy));
                *command = (char *) calloc(strlen(tempToken) + 1, sizeof(char));
                strcpy(*command, tempToken);
                tempToken = strstrip(strtok_r(NULL, ">", &copy));
                *redirectInPath = (char *) calloc(strlen(tempToken) + 1, sizeof(char));
                strcpy(*redirectInPath, tempToken);
                *redirectOutPath = (char *) calloc(strlen(copy) + 1, sizeof(char));
                strcpy(*redirectOutPath, strstrip(copy));
            }
            else {
                char * tempToken = strstrip(strtok_r(newString, ">", &copy));
                *command = (char *) calloc(strlen(tempToken) + 1, sizeof(char));
                strcpy(*command, tempToken);
                tempToken = strstrip(strtok_r(NULL, "<", &copy));
                *redirectOutPath = (char *) calloc(strlen(tempToken) + 1, sizeof(char));
                strcpy(*redirectOutPath, tempToken);
                *redirectInPath = (char *) calloc(strlen(copy) + 1, sizeof(char));
                strcpy(*redirectInPath, strstrip(copy));
            }
        }
        else if (strstr(s, "<") != NULL) {
            char * tempToken = strstrip(strtok_r(newString, "<", &copy));
            *command = (char *) calloc(strlen(tempToken) + 1, sizeof(char));
            strcpy(*command, tempToken);
            tempToken = strstrip(strtok_r(NULL, ">", &copy));
            *redirectInPath = (char *) calloc(strlen(tempToken) + 1, sizeof(char));
            strcpy(*redirectInPath, tempToken);
        }
        else if (strstr(s, ">") != NULL) {
            char * tempToken = strstrip(strtok_r(newString, ">", &copy));
            *command = (char *) calloc(strlen(tempToken) + 1, sizeof(char));
            strcpy(*command, tempToken);
            tempToken = strstrip(strtok_r(NULL, "<", &copy));
            *redirectOutPath = (char *) calloc(strlen(tempToken) + 1, sizeof(char));
            strcpy(*redirectOutPath, tempToken);
        }
        else {
            *command = (char *) calloc(strlen(s) + 1, sizeof(char));
            strcpy(*command, s);
        }
        free(newString);
        newString = NULL;
    }
}
