#include "simcpu.h"

static void printThread(void *print);
static void printBurst(void *print);
static void printProcess(void *print);

static bool isEmpty(char *line){
	for (int i=0;i<strlen(line);i++){
		if (line[i] != ' ' && line[i] != '\t'){
			return false;
		}
	}
	return true;
}

static char *getCopy(char *line){
	char *copy = malloc(sizeof(char)
		*(strlen(line)+1));
	strcpy(copy,line);

	return copy;
}

static void initEvent(Event *event){
	event->time = 0;
	event->threadNum = 0;
	event->procNum = 0;
	event->from = ready;
	event->to = ready;
}

static void initBurst(Burst *burst){
	burst->burstNum = 0;
	burst->cpuTm = 0;
	burst->IOTm = 0;
}

static void setBurst(Burst *burst,int cpuTm, int IOTm
	,int burstNum){

	burst->burstNum = burstNum;
	burst->cpuTm = cpuTm;
	burst->IOTm = IOTm;
}

static void initThread(Thread *thread){
	thread->threadNum =0;
	thread->burstCount=0;
	thread->arrivalTm=0;
	thread->burst = NULL;
}

static void initThreadRes(ThreadRes *res){
	res->processNum = 0;
	res->serviceTm=0;
	res->turnTm=0;
	res->finishTm=0;
	res->IOTm=0;
}
static void setThreadData(Thread *thread, int threadNum
	, int arrivalTm, int burstCount){

	thread->threadNum = threadNum;
	thread->burstCount = burstCount;
	thread->arrivalTm = arrivalTm;
}

static void setThreadRes(ThreadRes *res, int threadNum
	, int arrivalTm, int serviceTm, int turnTm
	, int finishTm, int IOTm, int processNum){

	res->processNum = processNum;
	res->threadNum = threadNum;
	res->arrivalTm = arrivalTm;
	res->serviceTm = serviceTm;
	res->turnTm = turnTm;
	res->finishTm = finishTm;
	res->IOTm = IOTm;
}

static void initProcess(Process *process){
	process->processNum = 0;
	process->threadCount = 0;
	process->thread = NULL;
}

static void setProcess(Process *process, int processNum
	, int threadCount){
	process->threadCount = threadCount;
	process->processNum = processNum;
}

static void initSimulationRes(SimulationRes *res){
	res->totalTm = 0;
	res->avgTurnTm = 0;
	res->cpuUtil = 0;
}

static void initSimulation(Simulation *sim){
	sim->processCount = 0;
	sim->sameSwitchTm = 0;
	sim->diffSwitchTm = 0;
	sim->process = NULL;
}

static void setSimulationRes(SimulationRes *res, int totalTm
	, double avgTurnTm, int cpuUtil){

	res->totalTm = totalTm;
	res->avgTurnTm = avgTurnTm;
	res->cpuUtil = cpuUtil;
}

static void setSimulationData(Simulation *sim,int processCount
	, int sameSwitchTm, int diffSwitchTm){

	sim->processCount = processCount;
	sim->sameSwitchTm = sameSwitchTm;
	sim->diffSwitchTm = diffSwitchTm;
}

static void freeElement(void *elem){
	free(elem);
}

static char *getLine(){
	char *line=NULL, curChar=' ';
	int charNum=0;

	while (curChar != '\n' && curChar != EOF){
		curChar = fgetc(stdin);
		if (feof(stdin) || curChar == '\n'){ break; }

		/*Allocate memory for line*/
		if (!line){ line = malloc(sizeof(char)*2); }
		else{
			line = realloc(line,sizeof(char)
				*(strlen(line)+2));
		}

		line[charNum] = curChar;
		line[charNum+1] = '\0';
		charNum++;
	}

	return line;
}

static Simulation *getSimulation(char *line){
	char *token=NULL,*lineCopy;
	int processCount,sameSwitchTm,diffSwitchTm;
	Simulation *simulation=malloc(sizeof(Simulation));

	initSimulation(simulation);
	lineCopy = getCopy(line);

	/*Get process count*/
	token = strtok(lineCopy," ");
	if (!token){
		fprintf(stderr,"Error: invalid format\n");
		exit(1);
	}
	processCount = strtol(token,NULL,10);

	/*Get time to switch to thread in same process*/
	token = strtok(NULL," ");
	if (!token){
		fprintf(stderr,"Error: invalid format\n");
		exit(1);
	}
	sameSwitchTm = strtol(token,NULL,10);

	/*Get time to switch to thread in different process*/
	token = strtok(NULL," ");
	if (!token){
		fprintf(stderr,"Error: invalid format\n");
		exit(1);
	}
	diffSwitchTm = strtol(token,NULL,10);

	/*Store simulator properties*/
	setSimulationData(simulation,processCount
		,sameSwitchTm,diffSwitchTm);
	
	free(lineCopy);
	return simulation;
}

static Process *getProcess(char *line){
	char *token=NULL,*lineCopy=NULL;
	int processNum,threadCount;
	Process *process=malloc(sizeof(Process));

	initProcess(process);
	lineCopy = getCopy(line);

	/*Get process number*/
	token = strtok(lineCopy," ");
	if (!token){
		fprintf(stderr,"Error: invalid format\n");
		exit(1);
	}
	processNum = strtol(token,NULL,10);

	/*Get number of threads in process*/
	token = strtok(NULL," ");
	if (!token){
		fprintf(stderr,"Error: invalid format\n");
		exit(1);
	}
	threadCount = strtol(token,NULL,10);

	/*Store process properties*/
	setProcess(process,processNum,threadCount);
	
	free(lineCopy);
	return process;
}

static Thread *getThread(char *line){
	char *token=NULL,*lineCopy;
	int threadNum, arrivalTm, burstCount;
	Thread *thread=malloc(sizeof(Thread));

	initThread(thread);
	lineCopy = getCopy(line);

	/*Get thread number*/
	token = strtok(lineCopy," ");
	if (!token){
		fprintf(stderr,"Error: invalid format\n");
		exit(1);
	}
	threadNum = strtol(token,NULL,10);

	/*Arrival time of thread*/
	token = strtok(NULL," ");
	if (!token){
		fprintf(stderr,"Error: invalid format\n");
		exit(1);
	}
	arrivalTm = strtol(token,NULL,10);

	/*Get the number of CPU bursts*/
	token = strtok(NULL," ");
	if (!token){
		fprintf(stderr,"Error: invalid format\n");
		exit(1);
	}
	burstCount = strtol(token,NULL,10);

	/*Store thread properties*/
	setThreadData(thread,threadNum,arrivalTm,burstCount);
	
	free(lineCopy);
	return thread;
}

static Burst *getBurst(char *line){
	char *token=NULL,*lineCopy;
	int burstNum=0,cpuTm=0,IOTm=0;
	Burst *burst=malloc(sizeof(Burst));

	initBurst(burst);
	lineCopy = getCopy(line);

	/*Get burst number*/
	token = strtok(lineCopy," ");
	if (!token){
		fprintf(stderr,"Error: invalid format\n");
		exit(1);
	}
	burstNum = strtol(token,NULL,10);

	/*CPU time of burst*/
	token = strtok(NULL," ");
	if (!token){
		fprintf(stderr,"Error: invalid format\n");
		exit(1);
	}
	cpuTm = strtol(token,NULL,10);

	/*IO time of burst*/
	token = strtok(NULL," ");
	/*Check if IO time is present*/
	if (token && !isEmpty(lineCopy)){
		IOTm = strtol(token,NULL,10);		
	}

	/*Store thread properties*/
	setBurst(burst,cpuTm,IOTm,burstNum);
	
	free(lineCopy);
	return burst;
}

static Simulation *fillSimProps(){
	char *line=NULL;
	int burstNum=0,threadNum=0,processNum=0;
	bool finished = false;
	Process *curProcess=NULL;
	Thread *curThread=NULL;
	Burst *curBurst=NULL;
	ParseState state=simulateprops;
	Simulation *sim;

	/*Get all the data from the file and store it
	inside data structures contained inside a 
	Simulation structure*/
	do{
		line = getLine();
		if (!line){ break; }

		/*Finished storing all thread properties*/	
		if (state == burstprops && burstNum == 0){
			if (threadNum > 0){				
				state = threadprops;
				threadNum--;
			}
			else if (processNum > 0){
				state = processprops;
				processNum--;
			}
		}
		/*Finished storing all thread properties*/	
		else if (state == threadprops && threadNum == 0){
			if (processNum > 0){				
				state = processprops;
				processNum--;
			}
		}
		else if (state == processprops){
			if (processNum == 0){
				finished = true;
			}
		}

		switch(state){

			case simulateprops:
				sim = getSimulation(line);
				state = processprops;
				processNum = sim->processCount;

				if (sim->processCount > 0){
					/*Create process list*/
					createList(&(sim->process));
				}

				break;

			case processprops:

				curProcess = getProcess(line);
				threadNum = curProcess->threadCount;

				if (curProcess->threadCount > 0){
					/*Create thread list*/
					if (!curProcess->thread){
						createList(&(curProcess->thread));
					}
				}

				/*Add process to list*/
				addToList(sim->process,curProcess);

				if (threadNum > 0){
					state = threadprops;
				}
				break;

			case threadprops:

				curThread = getThread(line);
				burstNum = curThread->burstCount;

				if (curThread->burstCount > 0){
					/*Create burst list*/
					if (!curThread->burst){
						createList(&(curThread->burst));
					}
				}

				/*Add thread to list*/
				addToList(curProcess->thread,curThread);

				/*Get next bursts properties*/
				if (burstNum > 0){
					state = burstprops;
				}

				break;

			case burstprops:

				/*Get next burst*/
				curBurst = getBurst(line);

				/*Add burst to list*/
				addToList(curThread->burst,curBurst);

				burstNum--;
				if (burstNum == 0){
					state = threadprops;	
					threadNum--;
				}

				break;
		}

		free(line);

	}while (!finished);

	return sim;
}

static void printProcess(void *print){
	Process *proc = (Process*)print;

	printf("processNum:%d\tthreadCount:%d\n"
		,proc->processNum,proc->threadCount);
}

static void printThread(void *print){
	Thread *thread = (Thread*)print;
	
	printf("threadNum:%d\tburstCount:%d\tarrivalTm:%d\n"
		,thread->threadNum,thread->burstCount,thread->arrivalTm);
}

static void printBurst(void *print){
	Burst *burst = (Burst*)print;

	printf("burstNum:%d\tcpuTm:%d\tIOTm:%d\n"
		,burst->burstNum,burst->cpuTm,burst->IOTm);
}

static void printSimulationData(Simulation *sim){
	Process *process;
	Thread *thread;
	Node *procNext;
	Node *threadNext;
	Node *burstNext;

	printf("-----Simulation Properties-----\n");
	printf("processCount:%d\tsameSwitchTm:%d\tdiffSwitchTm:%d\n"
		,sim->processCount,sim->sameSwitchTm,sim->diffSwitchTm);
	printf("--------------------------------\n");

	printf("-----------Process------------\n");

	/*Traverse processes and print each one*/
	procNext = getHead(sim->process);
	if (!procNext){ return; }

	do{
		printNode(*procNext,printProcess);

		/*Traverse threads and print each one*/
		process = getData(procNext);

		if (process->thread){
			threadNext = getHead(process->thread);

			do{
				printNode(*threadNext,printThread);

				/*Traverse bursts and print each one*/
				thread = getData(threadNext);
				if (thread->burst){
					burstNext = getHead(thread->burst);	
					do{
						printNode(*burstNext,printBurst);
						burstNext = getNext(burstNext);

					}while(burstNext);
				}

				threadNext = threadNext->next;

			}while(threadNext);

		}

		procNext = getNext(procNext);

	}while(procNext);

	printf("----------------------------------\n");
}

static void freeBursts(void *data){
	free(data);
}

static void freeThreads(void *data){
	Thread *thread = (Thread*)data;

	/*Free threads burst list*/
	freeList(&thread->burst,freeBursts);
	/*Free node data(process)*/
	free(data);
}

static void freeProcesses(void *data){
	Process *proc = (Process*)(data);
	
	/*Free processes thread list*/
	freeList(&proc->thread,freeThreads);
	/*Free node data(process)*/
	free(proc);
}

static void freeSimulation(Simulation **sim){
	freeList(&((*sim)->process),freeProcesses);
	free(*sim);
	*sim = NULL;
}

int threadCmp(const void *a,const void *b){
	Node *node1 = *((Node**)a);
	Node *node2 = *((Node**)b);
	Thread *thread1 = getData(node1);
	printf("thread1 arrtime: %d\n",thread1->arrivalTm);
	Thread *thread2 = getData(node2);
	return (thread1->arrivalTm - thread2->arrivalTm);
}

static Queue *sortThreads(Simulation *data){
	Queue *threads=NULL;
	Process *proc=NULL;
	Node **threadarr=NULL;	//Array of all thread node addresses
	Node *curThread=NULL;
	Node *node=NULL;
	int count=0,i=0;

	/*Create queue*/
	createQueue (&threads);

	/*Add threads from all processes to array*/
	node = getHead(data->process);
	while(node){
		proc = (Process*)getData(node);

		/*Check if threads present in process*/
		if (proc->threadCount > 0){
			count += proc->threadCount;

			/*Allocate memory*/
			if (!threads){
				threadarr = malloc(sizeof(Node*)
					*(count));
			}
			else{
				threadarr = realloc(threadarr
					,sizeof(Node*)*(count));
			}

			/*Add threads to array*/
			curThread = getHead(proc->thread);
			while (curThread){
				Thread *testThread = getData(curThread);
				threadarr[i] = curThread;
				i++;
				curThread = getNext(curThread);
			}

		}
		node = getNext(node);
	}
	
	/*Sort the array of thread nodes*/
	qsort(threadarr,count,sizeof(Node*),threadCmp);

	return threads;
}

static SimulationRes *simFCFS(Simulation *data){

	SimulationRes *res=malloc(sizeof(SimulationRes));
 	Queue *threads = NULL;
 	Node *node;
 	Thread *curThread;
 	SimState curState = new;
 	int curTime=0,arrTime=0,finishTime=0,servTime=0;

	/*Sort all threads by arrival time and
	 store them in queue.*/
	threads = sortThreads(data);


	while (!emptyQueue(threads)){
		/*Run thread for its CPU time*/
	}
	/*
	2. Simulate thread in the front of the queue until it
	blocks or finishes. Than preceed to simulate the next one
	after it until the queue is empty.

	while (!empty(threadQueue)){
		//Simulate CPU time
		curState = simulateThread(top(threadQueue)); //Will always return finished or blocked
		//Remove process from que if it finished
		if (curState == finished){
			//Create thread result node
			//Set thread result node to data
			//Store thread result node in list
			pop(threadQueue);
		}
		//Move process to back of queue
		else if (curState == blocked){
			thread = getTop(threadQueue);
			pop(threadQueue);
			addQue(threadQueue,thread);
		}
		//Store event here in list
	}

	*/
	return res;

}

static SimulationRes *simRR(Simulation *data){

	SimulationRes *res=malloc(sizeof(SimulationRes));
 	Queue *threads = NULL;

	/*Sort all threads by arrival time and
	 store them in queue.*/
	threads = sortThreads(data);


	/*
	2. Simulate thread in the front of the queue for the 
	given time quantum or until it blocks or finished.

	while (!empty(threadQueue)){
		//Simulate CPU time
		curState = simulateThreadRR(top(threadQueue)); //Will always return finished or blocked
		//Remove process from que if it finished
		if (curState == finished){
			//Create thread result node
			//Set thread result node to data
			//Store thread result node in list
			pop(threadQueue);
		}
		//Move process to back of queue
		else if (curState == blocked){
			thread = getTop(threadQueue);
			pop(threadQueue);
			addQue(threadQueue,thread);
		}
		//Store event here in list
	}

	*/
	return res;

}

int main(int argc, char **argv){
	bool detInfo=false,verbMode=false;
	int robQuantum;
	Simulation *simulation=NULL;
	SimulationRes *simResults=NULL;

	for (int i=0;i<argc;i++){
		if (!strcmp(argv[i],"-d")){
			detInfo = true;
		}
		else if (!strcmp(argv[i],"-v")){
			verbMode = true;
		}
		else if (!strcmp(argv[i],"-r")){
			if (argv[i+1]){
				robQuantum = strtol(argv[i+1],NULL,10);
			}
			else{
				fprintf(stderr,"Error: round robin enabled but time quantum not specified.\n");
				return(EXIT_FAILURE);
			}
		}
	}

	/*Get all simulation data from stdin*/
	simulation = fillSimProps();
	//printSimulationData(simulation);

	/*Run simulation using data gathered*/
	simFCFS(simulation);

	/*Free simulation data*/
	freeSimulation(&simulation);
	return 0;
}