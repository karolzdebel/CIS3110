#include "part2.h"

static void *copyProcess(void *data){
	Process *process = (Process*)data;
	Process *copy = malloc(sizeof(Process));

	copy->id = process->id;
	copy->size = process->size;

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

		push(processes,add,copyProcess);

		/*Free memory since push creates copy*/
		free(add);
	}

	return processes;
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

	destroyList(&processes,free);

	return EXIT_SUCCESS;

}