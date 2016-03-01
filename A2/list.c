#include "list.h"

/*********************************************************************
 * FUNCTION NAME: freeNodes
 * PURPOSE: Free the node, and all nodes after it.
 * ARGUMENTS: . Node that is linked to preceeding nodes(Node *node).
 *			  . Function that frees data in that node and takes
 * 				the data as an argument (void (*freeData)(void *)).
 *********************************************************************/
static void freeNodes(Node *node, void (*freeData)(void *));
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

static void freeNodes(Node *node, void (*freeData)(void *)){
	if (node->next){
		freeNodes(node->next,freeData);
	}
	freeData(node->data);
	free(node);
}

static void printNodes(Node *node, void (*printFunct)(void *)){
	printFunct(node->data);
	if (node->next){
		printNodes(node->next,printFunct);
	}
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

extern void freeList(List **list, void (*freeData)(void *)){
	if ((*list)->first){
		freeNodes((*list)->first,freeData);
	}
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