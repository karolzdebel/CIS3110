#include "simcpu.h"

//static void printThread(void *print);
//static void printBurst(void *print);
//static void printProcess(void *print);

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

// static void initStateEvent(StateEvent *event){
// 	event->time = 0;
// 	event->threadNum = 0;
// 	event->procNum = 0;
// 	event->from = ready;
// 	event->to = ready;
// }

static void setStateEvent(StateEvent *event, int time, int threadNum
	, int procNum, SimState from, SimState to){
	
	event->time = time;
	event->threadNum = threadNum;
	event->processNum = procNum;
	event->from = from;
	event->to = to;
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

// static void initSimEvent(SimEvent *event){
// 	event->threadNum=0;
// 	event->cpuTm=0;
// 	event->IOTm=0;
// 	event->processNum=0;
// 	event->sameSwitchTm=0;
// 	event->diffSwitchTm=0;
// }

static void setSimEvent(SimEvent *event,int arrivalTm
	,int cpuTm, int IOTm, int processNum, int sameSwitchTm
	, int diffSwitchTm,int threadNum){

	event->arrivalTm = arrivalTm;
	event->cpuTm = cpuTm;
	event->IOTm = IOTm;
	event->processNum = processNum;
	event->sameSwitchTm = sameSwitchTm;
	event->diffSwitchTm = diffSwitchTm;	
	event->threadNum = threadNum;
}

static void setThreadData(Thread *thread, int threadNum
	, int arrivalTm, int burstCount){

	thread->threadNum = threadNum;
	thread->burstCount = burstCount;
	thread->arrivalTm = arrivalTm;
}

// static void setThreadRes(ThreadRes *res, int threadNum
// 	, int arrivalTm, int serviceTm, int turnTm
// 	, int finishTm, int IOTm, int processNum){

// 	res->processNum = processNum;
// 	res->threadNum = threadNum;
// 	res->arrivalTm = arrivalTm;
// 	res->serviceTm = serviceTm;
// 	res->turnTm = turnTm;
// 	res->finishTm = finishTm;
// 	res->IOTm = IOTm;
// }

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
	res->threadRes = NULL;
}

static void initSimulation(Simulation *sim){
	sim->processCount = 0;
	sim->sameSwitchTm = 0;
	sim->diffSwitchTm = 0;
	sim->process = NULL;
}

// static void setSimulationRes(SimulationRes *res, int totalTm
// 	, double avgTurnTm, int cpuUtil){

// 	res->totalTm = totalTm;
// 	res->avgTurnTm = avgTurnTm;
// 	res->cpuUtil = cpuUtil;
// }

static void setSimulationData(Simulation *sim,int processCount
	, int sameSwitchTm, int diffSwitchTm){

	sim->processCount = processCount;
	sim->sameSwitchTm = sameSwitchTm;
	sim->diffSwitchTm = diffSwitchTm;
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

				/*Add burst to queue*/
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

// static void printProcess(void *print){
// 	Process *proc = (Process*)print;

// 	printf("processNum:%d\tthreadCount:%d\n"
// 		,proc->processNum,proc->threadCount);
// }

// // static void printThread(void *print){
// // 	Thread *thread = (Thread*)print;
	
// // 	printf("threadNum:%d\tburstCount:%d\tarrivalTm:%d\n"
// // 		,thread->threadNum,thread->burstCount,thread->arrivalTm);
// // }

// static void printBurst(void *print){
// 	Burst *burst = (Burst*)print;

// 	printf("burstNum:%d\tcpuTm:%d\tIOTm:%d\n"
// 		,burst->burstNum,burst->cpuTm,burst->IOTm);
// }

// static void printSimulationData(Simulation *sim){
// 	Process *process;
// 	Thread *thread;
// 	Node *procNext;
// 	Node *threadNext;
// 	Node *burstNext;

// 	printf("-----Simulation Properties-----\n");
// 	printf("processCount:%d\tsameSwitchTm:%d\tdiffSwitchTm:%d\n"
// 		,sim->processCount,sim->sameSwitchTm,sim->diffSwitchTm);
// 	printf("--------------------------------\n");

// 	printf("-----------Process------------\n");

// 	/*Traverse processes and print each one*/
// 	procNext = getHead(sim->process);
// 	if (!procNext){ return; }

// 	do{
// 		printNode(*procNext,printProcess);

// 		Traverse threads and print each one
// 		process = getData(procNext);

// 		if (process->thread){
// 			threadNext = getHead(process->thread);

// 			do{
// 				printNode(*threadNext,printThread);

// 				/*Traverse bursts and print each one*/
// 				thread = getData(threadNext);
// 				if (thread->burst){
// 					burstNext = getHead(thread->burst);	
// 					do{
// 						printNode(*burstNext,printBurst);
// 						burstNext = getNext(burstNext);

// 					}while(burstNext);
// 				}

// 				threadNext = threadNext->next;

// 			}while(threadNext);

// 		}

// 		procNext = getNext(procNext);

// 	}while(procNext);

// 	printf("----------------------------------\n");
// }

static void freeBursts(void *data){
	free(data);
}

static void freeThreads(void *data){
	Thread *thread = (Thread*)data;

	/*Free threads burst list*/
	if (thread->burst){
		freeListHard(&thread->burst,freeBursts);
	}
	/*Free node data(process)*/
	free(data);
}

static void freeProcesses(void *data){
	Process *proc = (Process*)(data);
	
	/*Free processes thread list*/
	if (proc->thread){
		freeListHard(&proc->thread,freeThreads);
	}
	/*Free node data(process)*/
	free(proc);
}

static void freeSimulation(Simulation **sim){
	if ((*sim)->process){
		freeListHard(&((*sim)->process),freeProcesses);
	}
	free(*sim);
	*sim = NULL;
}

static void freeSimulationRes(SimulationRes **sim){
	if ((*sim)->threadRes){
		freeListHard(&((*sim)->threadRes),free);
	}
	if ((*sim)->stateEvents){
		freeListHard(&((*sim)->stateEvents),free);
	}
	free(*sim);
	*sim = NULL;
}

int eventCmp(const void *a,const void *b){
	SimEvent *event1 = *((SimEvent**)a);
	SimEvent *event2 = *((SimEvent**)b);
	return (event2->arrivalTm - event1->arrivalTm);
}

static Queue *getSortedEvents(Simulation *data){
	int count=0,i=0;
	Queue *events=NULL;
	Process *proc=NULL;
	Node *curThread=NULL,*curBurst=NULL;
	Node *node=NULL;
	Thread *threadData=NULL;
	Burst *burstData=NULL;
	SimEvent *curEvent=NULL,**eventarr=NULL;

	/*Add threads from all processes to array*/
	node = getHead(data->process);
	while(node){
		proc = (Process*)getData(node);

		/*Check if threads present in process*/
		if (proc->threadCount > 0){

			/*Traverse threads*/
			curThread = getHead(proc->thread);
			while (curThread){
				threadData = getData(curThread);

				/*Allocate memory for array*/
				count += threadData->burstCount;
				if (!eventarr){
					eventarr = malloc(sizeof(SimEvent*)
						*(count));
				}
				else{
					eventarr = realloc(eventarr
						,sizeof(SimEvent*)*(count));
				}

				/*Traverse bursts adding each event queue member*/
				curBurst = getHead(threadData->burst);
				while(curBurst){
					burstData = getData(curBurst);

					/*Create and set data for event*/
					curEvent = malloc(sizeof(SimEvent));
					setSimEvent(curEvent,threadData->arrivalTm
						,burstData->cpuTm,burstData->IOTm
						,proc->processNum,data->sameSwitchTm
						,data->diffSwitchTm,threadData->threadNum);

					eventarr[i] = curEvent;
					i++;

					curBurst = getNext(curBurst);
				}
		
				curThread = getNext(curThread);
			}

		}
		node = getNext(node);
	}
	
	/*Sort the array of thread nodes*/
	qsort(eventarr,count,sizeof(SimEvent*),eventCmp);

	/*Create queue*/
	createQueue (&events);

	/*Push sorted thread nodes into queue*/
	for (i=0;i<count;i++){
		push(events,eventarr[i]);
	}
	free(eventarr);

	return events;
}

static List *waitSim(List *waitingList,Queue *readyQueue,int curTime){
	int pos=0;
	bool removed = false;
	List *eventInfo = NULL;
	Node *waitNode;
	SimEvent *waitEvent;
	StateEvent *stateEvent;
	SimState stateFrom=blocked,stateTo=blocked;

	createList(&eventInfo);

	/*Traverse waiting list*/
	waitNode = getHead(waitingList);
	while (waitNode){
		pos++;
		waitEvent = getData(waitNode);

		/*Simulate one time unit*/
		waitEvent->IOTm--;	
		assert(waitEvent->IOTm > -1);

		/*If event is done IO*/
		if (waitEvent->IOTm == 0){
			/*State of event is changing*/
			stateEvent = malloc(sizeof(StateEvent));

			if (waitEvent->cpuTm == 0){ stateTo = terminated; }
			else{ stateTo = ready; }

			/*Record state change and add to list*/
				waitEvent = getData(getListNode(waitingList,pos));
				setStateEvent(stateEvent,curTime,waitEvent->threadNum
					,waitEvent->processNum,stateFrom,stateTo);
				addToList(eventInfo,stateEvent);

			/*Event is complete therefore terminated*/
			if (waitEvent->cpuTm == 0){
				removeListHard(waitingList,pos,free);
			}

			/*Event goes back to ready queue since cpu time is left*/
			else{
				push(readyQueue,getData(getListNode(waitingList,pos)));
				removeListSoft(waitingList,pos);
			}
			removed = true;

		}

		/*If removed from list, get next event*/
		if (removed){
			waitNode = getHead(waitingList);
		}
		/*Get next*/
		else{
			waitNode = getNext(waitNode);
		}
		removed = false;
		
	}

	return eventInfo;
}

/*Returns 0 if not present*/
static ThreadRes *getThreadRes(List *threadRes, int processNum
	,int threadNum){

	int pos=0;
	Node *node = getHead(threadRes);
	ThreadRes *data;

	/*If empty return NULL*/
	if (!node){
		return NULL;
	}

	/*Traverse searching for match*/
	while(getNext(node)){
		pos++;
		data = getData(node);

		/*compare process and thread number*/
		if (data->processNum == processNum 
			&& data->threadNum == threadNum){

			return data;
		}
		node = getNext(node);
	}

	/*Return 0 if not found*/
	return NULL;
}

static SimulationRes *simFCFS(Simulation *data){

	int curTime = 0,curProcessNum=0,curThreadNum=0;
	SimulationRes *res;
 	Queue *eventQueue = NULL,*readyQueue=NULL;
 	SimEvent *curEvent=NULL;
 	Node *node,*simNode;
 	List *waitingList=NULL,*eventList = NULL;
 	ThreadRes *threadRes;

	res = malloc(sizeof(SimulationRes));
	initSimulationRes(res);

	createList(&waitingList);	
	createList(&(res->stateEvents));
	createList(&(res->threadRes));

	/*Get a queue of sorted events in order of arrival time*/
	eventQueue = getSortedEvents(data);

	while (!emptyList(waitingList)
		|| !emptyQueue(eventQueue)){

		/*Go through waiting list and decrease I/O time
		, get a list of state changes(events) that happened*/
		createQueue(&readyQueue);
		eventList = waitSim(waitingList,readyQueue,curTime);
		mergeList(res->stateEvents,eventList);

		/*Add ready bursts to event queue*/
		while (!emptyQueue(readyQueue)){
			node = pop(readyQueue);
			push(eventQueue,getData(node));
			free(node);
		}
		freeQueueSoft(&readyQueue);

		/*Decrease current bursts CPU time*/
		if (emptyQueue(eventQueue)){
			continue;
		}
		simNode = pop(eventQueue);
		curEvent = getData(simNode);
		curEvent->cpuTm--;

		/*Get thread result node associated with event*/
		curProcessNum = curEvent->processNum;
		curThreadNum = curEvent->threadNum;
		/*Found in list*/
		if (getThreadRes(res->threadRes
			,curProcessNum,curThreadNum)){

			/*Get the thread result from the list*/
			threadRes = getThreadRes(res->threadRes
				,curProcessNum,curThreadNum);
		}
		/*Not found in list*/
		else{
			/*Create and add to list*/
			threadRes = malloc(sizeof(ThreadRes));
			initThreadRes(threadRes);
			addToList(res->threadRes,threadRes);
		}


		/*Check whether burst is done*/
		if (curEvent->cpuTm == 0){
			/*Check if I/O time is present*/
			if (curEvent->IOTm > 0){
				/*Add to waiting list and remove from event queue*/
				addToList(waitingList,curEvent);
				free(simNode);
			}
			/*Remove event since it is done*/
			else{
				free(getData(simNode));
				free(simNode);
			}
		}
		else{
			push(eventQueue,getData(simNode));
			free(simNode);
		}
		curTime++;
	}

	/*Use free() since structures are empty*/
	free(eventQueue);
	free(waitingList);

	return res;

}

// static SimulationRes *simRR(Simulation *data){

// 	SimulationRes *res=malloc(sizeof(SimulationRes));
//  	Queue *threads = NULL;

// 	/*Sort all threads by arrival time and
// 	 store them in queue.*/
// 	threads = getSortedEvents(data);


	
// 	2. Simulate thread in the front of the queue for the 
// 	given time quantum or until it blocks or finished.

// 	while (!empty(threadQueue)){
// 		//Simulate CPU time
// 		curState = simulateThreadRR(top(threadQueue)); //Will always return finished or blocked
// 		//Remove process from que if it finished
// 		if (curState == finished){
// 			//Create thread result node
// 			//Set thread result node to data
// 			//Store thread result node in list
// 			pop(threadQueue);
// 		}
// 		//Move process to back of queue
// 		else if (curState == blocked){
// 			thread = getTop(threadQueue);
// 			pop(threadQueue);
// 			addQue(threadQueue,thread);
// 		}
// 		//Store event here in list
// 	}

	
// 	return res;

// }

static void printThreadRes(void *data){
	ThreadRes *thread = (ThreadRes*)data;

	printf("Thread %d of Process %d:\n"
		,thread->threadNum,thread->processNum );
	printf("arrival time: %d\n",thread->arrivalTm);
	printf("serivce time: %d units, I/O time: %d units"
		,thread->serviceTm,thread->IOTm);
	printf(", turnaround time: %d units, finish time: %d units\n"
		,thread->turnTm,thread->finishTm);
}

static void printSimState(SimState state){
	switch(state){
		case new:
			printf("new");
			break;
		case ready:
			printf("ready");
			break;
		case running:
			printf("running");
			break;
		case blocked:
			printf("blocked");
			break;
		case terminated:
			printf("terminated");
			break;	
	}
}

static void printStateEvent(void *data){
	StateEvent *event = (StateEvent*)data;

	printf("At time %d: Thread %d of Process %d moves from"
		,event->time,event->threadNum,event->processNum);
	printSimState(event->from);
	printf(" to ");
	printSimState(event->to);
	printf("\n");
}

static void printResults(SimulationRes *res
	, bool detInfo, bool verbMode, int timeQuant){

	if (timeQuant == 0){
		printf("FCFS Scheduling\n");
	}
	else{
		printf("Round Robin Scheduling(quantum = %d time units)\n"
			,timeQuant);
	}
	printf("Total Time required is: %d units\n",res->totalTm);
	printf("Average Turnaround Time is %f time units\n"
		,res->avgTurnTm);
	printf("CPU Utilization is %d%%",res->cpuUtil);

	if (detInfo){
		printList(*res->threadRes,printThreadRes);
	}
	if (verbMode){
		printList(*res->stateEvents,printStateEvent);
	}
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
	simResults = simFCFS(simulation);
	printResults(simResults,detInfo,verbMode
		,robQuantum);

	/*Free simulation data*/
	freeSimulation(&simulation);
	freeSimulationRes(&simResults);
	return 0;
}