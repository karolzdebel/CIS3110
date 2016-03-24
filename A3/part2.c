#include "part2.h"

static void *copyProcess(void *data){
	Process *process = (Process*)data;
	Process *copy = malloc(sizeof(Process));

	copy->id = process->id;
	copy->size = process->size;
	copy->address = process->address;
	copy->loads = process->loads;

	return (void*)copy;
}

static void *copyEvent(void *data){
	Event *event = (Event*)data;
	Event *copy = malloc(sizeof(Event));

	copy->id = event->id;
	copy->processes = event->processes;
	copy->holes = event->holes;
	copy->memUse = event->memUse;
	copy->cumMemUse = event->cumMemUse;

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
		add->loads = 0;

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
		if ( (memory[i]==_MEM_TAKEN || i==(_MEM_SIZE-1) )
			&& (memory[i-1]==_MEM_FREE) ){
			count++;
		}
	}
	return count;
}

static double memUsage(int memory[]){
	int count=0;

	for (int i=0;i<_MEM_SIZE;i++){
		if (memory[i]==_MEM_TAKEN){
			count++;
		}
	}

	return (((double)count)/((double)_MEM_SIZE))*100;
}

static int findNext(int start,int memSize, int memory[]){
	int memCounter=0;

	for (int i=start;i<_MEM_SIZE;i++){
		
		/*Look for empty memory*/
		if (memory[i]==_MEM_FREE){
			memCounter++;
		}
		else{ memCounter = 0; }
		/*Return index since address has been found*/
		if (memCounter==memSize){
			return (i+1)-memSize;
		}
	}

	return _MEM_FULL;
}

static void storeMemory(int address,int size, int memory[]){
	for (int i=address;i<address+size;i++){
		memory[i] = 1;
	}
}

static void clearMemory(int address,int size, int memory[]){
	for (int i=address;i<address+size;i++){
		memory[i] = 0;
	}
}

static void createMemory(int memory[]){
	for (int i=0;i<_MEM_SIZE;i++){
		memory[i]=0;
	}
}

static double getCumulativeMem(List *events, double memUse){
	double total = 0;
	Event *event;
	for (int i=0;i<events->count;i++){
		event = get(events,i,copyEvent);
		total += event->memUse;
		free(event);
	}
	total += memUse;

	return (total/((double)(events->count+1)));
}

static Event *createEvent(Process *process,List *events
		,int memory[], int processCount){

	Event *event = malloc(sizeof(Event));

	event->id = process->id;
	event->processes = processCount;
	event->holes = holeCount(memory);
	event->memUse = memUsage(memory);
	event->cumMemUse 
		= getCumulativeMem(events,event->memUse);

	return event;

}

static List *firstNext(int memory[], List *waiting){
	List *inMemory,*finished,*events;
	Process *top,*longest,*add;
	Event *event;
	int address, processCount=0;

	/*Cannot simulate empty list*/
	if (empty(waiting)){ return NULL; }

	/*Queue of processes in memory*/
	createList(&events);
	createList(&inMemory);
	createList(&finished);
	
	/*While not all processes are finished*/
	do{

		processCount = inMemory->count+waiting->count;

		top = getTop(waiting,copyProcess);
		address = findNext(0,top->size,memory);
		free(top);

		/*No space for process*/
		if (address == _MEM_FULL){

			/*Remove longest stored process from memory*/
			longest = (Process*)pop(inMemory);
			clearMemory(longest->address,longest->size,memory);

			/*If 3 loads than remove*/
			if (longest->loads == 3){
				push(finished,longest,copyProcess);
				free(longest);
			}
			/*Otherwise store in waiting list*/
			else{
				push(waiting,longest,copyProcess);	
				free(longest);	
			}
		}
		/*Space found for process*/
		else{

			/*Add head of waiting list to memory*/
			add = (Process*)pop(waiting);
			add->loads++;
			add->address = address;
			storeMemory(add->address,add->size,memory);
			push(inMemory,add,copyProcess);

			/*Store event*/
			event = createEvent(add,events,memory,processCount);
			push(events,event,copyEvent);
			free(event);

			free(add);
		}

	}while(!empty(waiting));

	destroyList(&finished,free);
	destroyList(&inMemory,free);

	return events;
}

static void printEvents(List *events){
	Event *event;

	for (int i=1;i<=events->count;i++){
		event = get(events,i,copyEvent);

		printf("%c loaded, #processes = %d"
		", $holes = %d, %%memusage = %.1lf"
		", cumulative%%mem = %.1lf\n"
		,event->id,event->processes
		,event->holes,event->memUse
		,event->cumMemUse);		

		free(event);
	}
}

int main(int argc, char **argv){
	FILE *file;
	List *processes,*events;
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

	events = firstNext(memory,processes);
	printEvents(events);

	destroyList(&events,free);
	destroyList(&processes,free);

	return EXIT_SUCCESS;

}