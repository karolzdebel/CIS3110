#include "list.h"

/*********************************************************************
 * FUNCTION NAME: initNode
 * PURPOSE: Initialize a node.
 * ARGUMENTS: . Node to be initialized(Node node).
 * RETURNS: CalError based on if a 'V' was found.
 *********************************************************************/
static void initNode(Node node);
/*********************************************************************
 * FUNCTION NAME: printNodes
 * PURPOSE: Print node, and all the following nodes in order.
 * ARGUMENTS: . Node that is linked to preceeding nodes(Node *node).
 *			  . Function that prints data in that node and takes
 * 				the data as an argument (void (*printFunct)(void *)).
 *********************************************************************/
static void printNodes(Node *node, void (*printFunct)(void *));

static void printNodes(Node *node, void (*printFunct)(void *)){
	printFunct(node->data);
	if (node->next){
		printNodes(node->next,printFunct);
	}
}

extern Node *getHead(List *list){
	return list->first;
}

extern Node *getNext(Node *node){
	return node->next;
}

extern void *getData(Node *node){
	return node->data;
}

extern void addToList(List *list,void *add){
	Node *next;

	/*Check if first element exists*/
	if (!list->first){
		/*Add to first position*/
		list->first = malloc(sizeof(Node));
		list->first->next = NULL;
		list->first->data = add;
	}
	
	else{
		/*Traverse to last position in the list*/
		next = list->first;
		while (next->next){
			next = next->next;
		}
		/*Add to list*/
		next->next = malloc(sizeof(Node));
		next->next->data = add;
		next->next->next = NULL;
	}

	list->count++;
}

extern void freeNodes(Node *node, void (*freeData)(void *)){
	if (node->next){
		freeNodes(node->next,freeData);
	}
	freeData(node->data);
	free(node);
}

extern void freeList(List **list, void (*freeData)(void *)){
	if ((*list)->first){
		freeNodes((*list)->first,freeData);
	}
	free(*list);
}

static void initNode(Node node){
	node.data = NULL;
	node.next = NULL;
}

extern void createList(List **list){
	*list = malloc(sizeof(List));
	(*list)->count = 0;
	(*list)->first = NULL;
}

extern void printList(List list, void (*printFunct)(void*)){
	if (list.first){
		printNodes(list.first,printFunct);
	}
}

extern void printNode(Node node, void (*printFunct)(void*)){
 	printFunct(node.data);
}

extern void createQueue(Queue **queue){
	*queue = malloc(sizeof(Queue));
	(*queue)->count = 0;
	(*queue)->first = NULL;
	(*queue)->last = NULL;
}

extern bool emptyQueue(Queue *queue){
	return (queue->count == 0);
}
extern void push(Queue *que, Node *node){
	Node *temp = que->last;
	
	if (!que->first){
		que->first = node;
		que->last = node;
	}

	que->last = node;
	node->next = temp;
	que->count++;
}

extern Node *pop(Queue *que){
	Node *temp = que->last;
	que->last = que->last->next;
	que->count--;

	return temp;
}