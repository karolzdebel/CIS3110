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
 * 			  . Function that prints node data.
 *********************************************************************/
extern void printList(List list, void (*printFunct)(void*));

#define _LIST_H
#endif