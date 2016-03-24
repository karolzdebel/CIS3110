#include "part2.h"

static void *copyProcess(void *data){
	Process *process = (Process*)data;
	Process *copy = malloc(sizeof(Process));

	copy->id = process->id;
	copy->size = process->size;
	copy->address = process->address;

	return (void*)copy;
}

static bool isEmpty(char *line){
	
	for (int i=0;i<strlen(line);i++){
		if (line[i] != ' ' && line[i] != '\t'){
			return false;
		}
	}
	return true;
}

static List *getProcesses(FILE *file){
	char id,*token,line[_SIZE_LINE];
	int size;
	Process *add;
	List *processes;

	createList(&processes);

	while (fgets(line,_SIZE_LINE,file)){

		/*Skip any empty lines*/
		if (isEmpty(line)){ continue; }

		/*Get id*/
		token = strtok(line," ");
		id = token[0];

		/*Get size*/
		token = strtok(NULL," ");
		size = (int)strtol(token,NULL,10);

		/*Store data inside process*/
		add = malloc(sizeof(Process));
		add->id = id;
		add->size = size;
		add->address = 0;

		push(processes,add,copyProcess);

		/*Free memory since push creates copy*/
		free(add);
	}

	return processes;
}

static int holeCount(int memory[]){
	int count=0;

	for (int i=1;i<_MEM_SIZE;i++){

		/*If gap is found between blocks or block and 
		end of memory than increaes hole counter*/
		if (memory[i]==_MEM_TAKEN 
			&& (memory[i-1]==_MEM_FREE || i==(_MEM_SIZE-1))){
			count++;
		}
	}
	return count;
}

static double memUsage(int memory[]){
	int count=0;

	for (int i=1;i<_MEM_SIZE;i++){
		if (memory[i]==_MEM_TAKEN){
			count++;
		}
	}

	return (((double)count)/((double)_MEM_SIZE))*100;
}

static double getAverage(double values[],int size){
	double total=0;
	for (int i=0;i<size;i++){
		total+=values[i];
	}

	return (total/((double)size)); 
}

static int findNext(int start,int memSize, int memory[]){
	int memCounter=0;

	for (int i=start;i<_MEM_SIZE;i++){
		
		/*Look for empty memory*/
		if (memory[i]==_MEM_FREE){
			memCounter++;
		}
		/*Return index since address has been found*/
		if (memCounter==memSize){
			return i;
		}
	}

	return _MEM_FULL;
}

static void createMemory(int memory[]){
	for (int i=0;i<_MEM_SIZE;i++){
		memory[i]=0;
	}
}

static void firstNext(int memory[], List *processes){
	printf("nothing yet\n");
}

int main(int argc, char **argv){
	FILE *file;
	List *processes;
	int memory[_MEM_SIZE];

	if (argc != 2){
		fprintf(stderr,_ERR0_MSG);
		return EXIT_FAILURE;
	}

	file = fopen(argv[1],"r");
	if (!file){
		fprintf(stderr,_ERR1_MSG);
		return EXIT_FAILURE;
	}

	processes = getProcesses(file);
	fclose(file);

	createMemory(memory);

	destroyList(&processes,free);

	return EXIT_SUCCESS;

}