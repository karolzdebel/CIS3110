#include "list.h"

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

extern void removeListNode(List *list,int pos){
	Node *next,*temp;

	/*Remove from position 1*/
	if (pos == 1){
		temp = list->first;
		if (list->count > 1){
			list->first = list->first->next;
		}
		else{
			list->first = NULL;
		}
		free(temp);
		list->count--;
		return;
	}

	/*Remove from middle or end*/
	next = getHead(list);
	for (int i=0;i<pos-2;i++){
		next = getNext(next);
	}
	temp = next->next;
	/*Remove from middle*/
	if (pos < list->count){
		next->next = next->next->next;
	}
	/*Remove from last*/
	else{
		next->next = NULL;
	}
	free(temp);
	list->count--;
}

extern Node *getListNode(List *list,int pos){
	Node *next;

	next = getHead(list);
	for (int i=0;i<pos-1;i++){
		next = getNext(next);
	}
	return next;
}

extern void freeNodesHard(Node *node, void (*freeData)(void *)){
	if (node->next){
		freeNodesHard(node->next,freeData);
	}
	freeData(node->data);
	free(node);
}

extern void freeNodesSoft(Node *node){
	if (node->next){
		freeNodesSoft(node->next);
	}
	free(node);
}

extern void freeListHard(List **list, void (*freeData)(void *)){
	if ((*list)->first){
		freeNodesHard((*list)->first,freeData);
	}
	free(*list);
}

extern void freeListSoft(List **list){
	if ((*list)->first){
		freeNodesSoft((*list)->first);
	}
	free(*list);
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

extern bool emptyList(List *list){
	return (list->count == 0);
}

extern void push(Queue *que, void *data){
	Node *temp,*add;
	
	add = malloc(sizeof(Node));
	add->data = data;
	add->next = NULL;

	if (!que->first){
		que->first = add;
		que->last = add;
	}
	else{
		temp = que->last;
		que->last = add;
		add->next = temp;
	}

	que->count++;
}

extern Node *pop(Queue *que){

	if (que->count == 0){
		return NULL;
	}

	Node *temp = que->last;
	que->last = que->last->next;
	if (que->count == 1){
		que->first = NULL;
	}
	que->count--;

	return temp;
}

extern Node *getFirst(Queue *queue){
	return queue->first;
}

extern void freeQueueHard(Queue **queue, void (*freeData)(void *)){
	if ((*queue)->last){
		freeNodesHard((*queue)->last,freeData);
	}
	free(*queue);
}
extern void freeQueueSoft(Queue **queue){
	Node *next;
	next = pop(*queue);
	while(next){
		free(next);
		next = pop(*queue);
	}
	free(*queue);
}