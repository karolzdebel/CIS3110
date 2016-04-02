#include <stdlib.h>
#include <stdio.h>
#include "list.h"

static void printNum(void *data){
	int *num = (int*)data;

	printf("num: %d\n",*num);
}

int main(int argc, char **argv){
	List *list;
	void *popData;
	int **data;
	int pos,size;

	if (argc != 3){
		printf("Error, incorrect number of arguments\n");
		printf("./a.out [size] [removePos]\n");
		exit(1);
	}

	size = strtol(argv[1],NULL,10);
	pos = strtol(argv[2],NULL,10);

	if (pos > size){
		printf("Error pos greater than size!\n");
		exit(1);
	}

	data = malloc(sizeof(int*)*size);

	createList(&list);
	for (int i=1;i<size+1;i++){
		data[i-1] = malloc(sizeof(int));
		*(data[i-1]) = i;
		push(list,data[i-1]);
	}
	// for (int i=1;i<size+1;i++){
	// 	popData = pop(list);
	// 	printf("Popped node %d, top:%d\n",i,*((int*)(popData)));
	// 	free(popData);
	// }
	free(data);
	removeSoft(list,pos);
	printf("List after removal:\n");
	printList(*list,printNum);

}