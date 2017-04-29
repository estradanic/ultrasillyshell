#include "listUtils.h"

Node * buildNode(FILE * fin, void *(*buildData)(FILE * in) ) {
	Node * newNode = (Node *)calloc(1,sizeof(Node));
	newNode->data = buildData(fin);
	return newNode;
}

Node * buildNode_Type(void * passedIn) {
	Node * newNode = (Node *)calloc(1,sizeof(Node));
	newNode->data = passedIn;
	return newNode;
}

void sort(LinkedList * theList, int (*compare)(const void *, const void *)) {
	if (theList == NULL){
        perror("theList can't be null");
    	exit(-99);
    }
	else if (theList->size != 0 && theList->size != 1) {
		Node * index, * index2, * smallest;
		void * temp;
		for(index = theList->head->next; index != NULL; index = index->next) {
			smallest = index;
			for(index2 = index; index2 != NULL; index2 = index2->next) {
			    if (compare(smallest->data,index2->data) > 0) {
				smallest = index2;
			    }
			}
			temp = index->data;
			index->data = smallest->data;
			smallest->data = temp;
		}
	}
}

void buildList(LinkedList * myList, int total, FILE * fin, void * (*buildData)(FILE * in))  {
	if (myList == NULL){
        perror("myList can't be null");
		exit(-99);
    }
	else if (total <= 0){
        perror("total must be greater than 0");
		exit(-99);	
    }
	else {
        int index;
		for (index = 0; index < total; index++)
		{
			void * temp = buildData(fin);
			Node * newNode = (Node *)calloc(1,sizeof(Node));
			newNode->data = temp;
			addFirst(myList, newNode);
		}
		myList->size = total;
	}
}
