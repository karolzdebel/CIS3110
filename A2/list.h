#ifndef _LIST_H

#include <stdio.h>
#include <stdlib.h>

/*List node*/
typedef struct Node {
	void *data;		//Address of element in the list
	struct Node *next;			//Next node in the list
} Node;

/*List of elements*/
typedef struct List {
	int count;			//Number of proccesses in the list
	Node *first;		//First element in the list
} List;

/*********************************************************************
 * FUNCTION NAME: addToList
 * PURPOSE: Add element to the list.
 * ARGUMENTS: . List for the element to be added to(List *list).
 *			  . Element to be added(void *add).
 *********************************************************************/
extern void addToList(List *list,void *add);
/*********************************************************************
 * FUNCTION NAME: initList
 * PURPOSE: Initialize a list.
 * ARGUMENTS: . List to be initialized(List list).
 *********************************************************************/
extern void createList(List **list);
/*********************************************************************
 * FUNCTION NAME: createList
 * PURPOSE: Free memory allocated to list.
 * ARGUMENTS: . List to be free'd (List **list).
 * 			  . Function that frees node data.
 *********************************************************************/
extern void freeList(List **list, void (*freeData)(void *));
/*********************************************************************
 * FUNCTION NAME: printList
 * PURPOSE: Print all list data.
 * ARGUMENTS: . List to be printed (List list).
 * 			  . Function that prints node 
 *              data(void (*printFunct)(void*)).
 *********************************************************************/
extern void printList(List list, void (*printFunct)(void*));
/*********************************************************************
 * FUNCTION NAME: printNode
 * PURPOSE: Print node data.
 * ARGUMENTS: . Node to be printed (Node node).
 * 			  . Function that prints node 
 *			    data (void (*printFunct)(void*)).
 *********************************************************************/
 extern void printNode(Node node, void (*printFunct)(void*));
/*********************************************************************
 * FUNCTION NAME: getHead
 * PURPOSE: Gets the head of the list.
 * ARGUMENTS: . Address of the list which contains the head(List *list).
 * RETURNS:   . Address of the head of the list(Node *).
 *********************************************************************/
extern Node *getHead(List *list);
/*********************************************************************
 * FUNCTION NAME: getNext
 * PURPOSE: Gets next node in the list.
 * ARGUMENTS: . Address of the previous node to the one you
 *              want(Node node).
 * RETURNS:   . Address of the next node in the list(Node *).
 *********************************************************************/
extern Node *getNext(Node *node);
/*********************************************************************
 * FUNCTION NAME: getData
 * PURPOSE: Get the data stored inside a node.
 * ARGUMENTS: . Address of the node which stores the data(Node *node).
 * RETURNS:   . Address of the data(void *).
 *********************************************************************/
extern void *getData(Node *node);

#define _LIST_H
#endif