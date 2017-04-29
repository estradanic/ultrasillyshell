#include "linkedList.h"


LinkedList * linkedList() {
	Node * head = (Node *)calloc(1, sizeof(Node));
	LinkedList * list = (LinkedList *)calloc(1,sizeof(LinkedList));
	list->head = head;
	return list;
}

void addLast(LinkedList * theList, Node * newNode) {
	if (theList == NULL){
        perror("theList can't be null");
		exit(-99);
    }
	else if (newNode == NULL){
        perror("newNode can't be null");
		exit(-99);
    }
	else {
		Node * current = theList->head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = newNode;
		theList->size = theList->size + 1;
	}
}

void addFirst(LinkedList * theList, Node * newNode) {
	if (theList == NULL){
        perror("theList can't be null");
		exit(-99);
    }

	else if (newNode == NULL){
        perror("newNode can't be null");
		exit(-99);
    }
	else {	
		Node * head = theList->head;
		newNode->next = head->next;
		head->next = newNode;
		theList->size = theList->size + 1;
	}
}

void removeFirst(LinkedList * theList, void (*removeData)(void *)) {
	if (theList == NULL){
        perror("theList can't be null");
		exit(-99);
    }
	else {
		Node * head = theList->head;
		Node * next = head->next->next;
		Node * removed = head->next;
		head->next = next;
		removeData(removed->data);
		free(removed);
		removed = NULL;
		theList->size = (theList->size) - 1;
	}
}

void * retrieveFirst(LinkedList * theList) {
	if (theList == NULL){
        perror("theList can't be null");
		exit(-99);
    }
	else{
		return theList->head->next;
	}
}

void * retrieveNthLast(LinkedList * theList, int num) {
	if (theList == NULL){
        perror("theList can't be null");
		exit(-99);
    }
	else {
		Node * current = theList->head;
		int index;
		for (index = 0; index < theList->size - num; index++) {
			current = current->next;
		}
		return current;
	}
}

void * retrieveNth(LinkedList * theList, int num) {
	if (theList == NULL){
        perror("theList can't be null");
		exit(-99);
    }
	else {
		Node * current = theList->head;
		int index;
		for (index = 0; index < num; index++) {
			current = current->next;
		}
		return current;
	}
}

int findInList(LinkedList * theList, void * toFind, int (*compare)(const void *, const void *)) {
	Node * nodeToFind = (Node *)toFind;
	if (theList == NULL){
        perror("theList can't be null");
		exit(-99);
    }
	else {
		Node * current = theList->head->next;
		while (current != NULL) {
			if (compare(current->data, nodeToFind->data) == 0) {
				toFind = current;
				return 1;
			}
			else {
				current = current->next;
			}
		}
		return 0;
	}
}


void removeItem(LinkedList * theList, Node * newNode, void (*removeData)(void *), int (*compare)(const void *, const void *)) {
    void * toRemove = newNode->data;
	if (theList == NULL){
        perror("theList can't be null");
		exit(-99);
    }
	else if (newNode == NULL){
        perror("newNode can't be null");
		exit(-99);
    }
	else {
		Node * current = theList->head;
		Node * temp;
		if (current->next == NULL) 
			printf("Empty List");
		else {
		    Node * prev = theList->head;
		    current = current->next;
		    while (current != NULL) {
				if (compare(current->data, toRemove) == 0) {
					temp = current;
					prev->next = current->next;
					current = current->next;

					theList->size = theList->size - 1;

					removeData(temp->data);
					free(temp);
					temp = NULL;
				}
				else {
					prev = prev->next;
					current = current->next;
				}
		    }
		}
	}
	removeData(toRemove);
	free(newNode);
	newNode = NULL;
}


void clearList(LinkedList * theList, void (*removeData)(void *)) {
	if (theList == NULL){
        perror("theList can't be null");
		exit(-99);
    }
	else {
		Node * current = theList->head->next;
		Node * temp;
		while (current != NULL) {
			temp = current;
			current = current->next;
			removeData(temp->data);
			free(temp); 
			temp = NULL;
		}
		free(theList->head);
		theList->head = NULL;
		theList->size = 0;
	}
}

void printList(const LinkedList * theList, void (*convertData)(void *), int start) {
	if (theList == NULL){
		printf("Empty List");
    }
	else {
		if (start < 0)
			start = 0;
		Node * current = theList->head->next;
		int index;
		for (index = 0; index < start; index++)
			current = current->next;
		while (current != NULL) {
			convertData(current->data);
			current = current->next;
		}
		printf("\n");
	}
}
