#ifndef _LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*List node*/
typedef struct Node {
	void *data;		//Address of element in the list
	struct Node *next;			//Next node in the list
} Node;

/*List of elements*/
typedef struct List {
	int count;			//Number of elements in the list
	Node *first;		//First element in the list
} List;

/*Queue of elements*/
typedef struct Queue{
	int count;			//Number of elements in the queue
	Node *first;		//First element in the queue
	Node *last;			//Last element in the queeu
} Queue;

/*********************************************************************
 * FUNCTION NAME: addToList
 * PURPOSE: Add element to the list.
 * ARGUMENTS: . List for the element to be added to(List *list).
 *			  . Element to be added(void *add).
 *********************************************************************/
extern void addToList(List *list,void *add);
/*********************************************************************
 * FUNCTION NAME: removeListNode
 * PURPOSE: Remove node from the list without freeing the data.
 * ARGUMENTS: . List the node is being removed from(List *list).
 *			  . Position to be removed from (int pos).
 *********************************************************************/
extern void removeListNode(List *list,int pos);
/*********************************************************************
 * FUNCTION NAME: createList
 * PURPOSE: Free memory allocated to list.
 * ARGUMENTS: . List to be free'd (List **list).
 * 			  . Function that frees node data.
 *********************************************************************/
extern void createList(List **list);
/*********************************************************************
 * FUNCTION NAME: getListNode
 * PURPOSE: Get any node in the list.
 * ARGUMENTS: . List which contains the node (List **list).
 * 			  . Position of the node.
 * RETURNS: Address of the node found at given position
 *			in the list(Node *).
 *********************************************************************/
extern Node *getListNode(List *list,int pos);
/*********************************************************************
 * FUNCTION NAME: freeListHard
 * PURPOSE: Free memory allocated to list and all data.
 * ARGUMENTS: . List to be free'd (List **list).
 * 			  . Function that frees node data.
 *********************************************************************/
extern void freeListHard(List **list, void (*freeData)(void *));
/*********************************************************************
 * FUNCTION NAME: freeListSoft
 * PURPOSE: Free memory allocated to list but not data.
 * ARGUMENTS: . List to be free'd (List **list).
 *********************************************************************/
extern void freeListSoft(List **list);
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
/*********************************************************************
 * FUNCTION NAME: freeNodes
 * PURPOSE: Free the node, and all nodes after it.
 * ARGUMENTS: . Node that is linked to preceeding nodes(Node *node).
 *			  . Function that frees data in that node and takes
 * 				the data as an argument (void (*freeData)(void *)).
 *********************************************************************/
extern void freeNodes(Node *node, void (*freeData)(void *));
/*********************************************************************
 * FUNCTION NAME: createQueue
 * PURPOSE: Initialize and allocate memory for a queue.
 * ARGUMENTS: . Address of the pointer to the queue to be 
 *				created(Queue **queue).
 *********************************************************************/
extern void createQueue(Queue **queue);
/*********************************************************************
 * FUNCTION NAME: pop
 * PURPOSE: Remove last node in the queue.
 * ARGUMENTS: . Queue that the node is being removed 
 *              from(Queue *que).
 * RETURNS: Address of the node that was removed (Node *).
 *********************************************************************/
extern Node *pop(Queue *queue);
/*********************************************************************
 * FUNCTION NAME: push
 * PURPOSE: Add a node to the queue.
 * ARGUMENTS: . Queue the node is being added to(Queue *queue).
 *			  . Node that is being added to the queue(Node *node).
 *********************************************************************/
extern void push(Queue *queue, void *node);
/*********************************************************************
 * FUNCTION NAME: emptyQueue
 * PURPOSE: Checks whether queue is empty.
 * ARGUMENTS: . Queue being checked for empty(Queue *queue).
 * RETURNS: Whether the queue is empty(Bool).
 *********************************************************************/
extern bool emptyQueue(Queue *queue);
/*********************************************************************
 * FUNCTION NAME: top
 * PURPOSE: Gets the first node in the queue.
 * ARGUMENTS: . Queue that contains the node(Queue *queue).
 * RETURNS: Address of the first node in the queue(Node *).
 *********************************************************************/
extern Node *getFirst(Queue *queue);
/*********************************************************************
 * FUNCTION NAME: freeQueueHard
 * PURPOSE: Free memory allocated for a queue and all data.
 * ARGUMENTS: . Pointer to the address of the queue to be 
 *				freed(Queue **queue).
 *			  . Function which frees node 
 *				data(void (*freeData)(void *)).
 *********************************************************************/
extern void freeQueueHard(Queue **queue, void (*freeData)(void *));
/*********************************************************************
 * FUNCTION NAME: freeQueueSoft
 * PURPOSE: Free memory allocated for a queue but not the data.
 * ARGUMENTS: . Pointer to the address of the queue to be 
 *				freed(Queue **queue).
 *********************************************************************/
extern void freeQueueSoft(Queue **queue);
/*********************************************************************
 * FUNCTION NAME: emptyList
 * PURPOSE: Checks whether list is empty.
 * ARGUMENTS: . List to be checked(List *list).
 * RETURNS: Whether the list is empty(Bool).
 *********************************************************************/
extern bool emptyList(List *list);

#define _LIST_H
#endif