#include "list.h"
#include "simcpu.h"

static void printThread(void *print){
	Thread *thread = (Thread*)print;
	
	printf("processNum:%d\tthreadNum:%d\tburstCount:%d\tarrivalTm:%d\n"
		,thread->processNum,thread->threadNum,thread->burstCount,thread->arrivalTm);
}

static void printBurst(void *print){
	Burst *burst = (Burst*)print;

	printf("burstNum:%d\tcpuTm:%d\tIOTm:%d\n"
		,burst->burstNum,burst->cpuTm,burst->IOTm);
}

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

 static void initBurst(Burst *burst){
	burst->burstNum = 0;
	burst->cpuTm = 0;
	burst->IOTm = 0;
}

static void initThread(Thread *thread){
	thread->threadNum =0;
	thread->burstCount=0;
	thread->arrivalTm=0;
	thread->serviceTm=0;
	thread->turnTm=0;
	thread->finishTm=0;
	thread->IOTm=0;
	thread->burst = NULL;
}

static void setThreadFileData(Thread *thread, int threadNum
	, int processNum, int arrivalTm, int burstCount){

	thread->processNum = processNum;
	thread->threadNum = threadNum;
	thread->burstCount = burstCount;
	thread->arrivalTm = arrivalTm;
}

static void setBurst(Burst *burst,int cpuTm, int IOTm
	,int burstNum){

	burst->burstNum = burstNum;
	burst->cpuTm = cpuTm;
	burst->IOTm = IOTm;
}

static void setSimulationData(Simulation *sim, int processCount
	,int sameSwitchTm, int diffSwitchTm){

	sim->processCount = processCount;
	sim->sameSwitchTm = sameSwitchTm;
	sim->diffSwitchTm = diffSwitchTm;
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

static Thread *getThread(char *line, int processNum){
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
	setThreadFileData(thread,threadNum,processNum
		,arrivalTm,burstCount);
	
	free(lineCopy);
	return thread;
}

static void getProcessInfo(char *line
	, int *processNum, int *threadCount){
	char *token=NULL,*lineCopy=NULL;

	lineCopy = getCopy(line);

	/*Get process number*/
	token = strtok(lineCopy," ");
	if (!token){
		fprintf(stderr,"Error: invalid format\n");
		exit(1);
	}
	*processNum = strtol(token,NULL,10);

	/*Get number of threads in process*/
	token = strtok(NULL," ");
	if (!token){
		fprintf(stderr,"Error: invalid format\n");
		exit(1);
	}
	*threadCount = strtol(token,NULL,10);

	free(lineCopy);
}

static void initSimulation(Simulation *sim){
	sim->processCount = 0;
	sim->sameSwitchTm = 0;
	sim->diffSwitchTm = 0;
	sim->threads = NULL;
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

static Simulation *fillSimProps(){
	char *line=NULL;
	int burstNum=0,threadCounter=0,processCounter=0;
	int processNum=0;
	bool finished = false;
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
			if (threadCounter > 0){				
				state = threadprops;
				threadCounter--;
			}
			else if (processCounter > 0){
				state = processprops;
				processCounter--;
			}
		}
		/*Finished storing all thread properties*/	
		else if (state == threadprops && threadCounter == 0){
			if (processCounter > 0){				
				state = processprops;
				processCounter--;
			}
		}
		else if (state == processprops){
			if (processCounter == 0){
				finished = true;
			}
		}

		switch(state){

			case simulateprops:
				sim = getSimulation(line);
				state = processprops;
				processCounter = sim->processCount;

				if (sim->processCount > 0){
					/*Create process list*/
					createList(&(sim->threads));
				}

				break;

			case processprops:

				getProcessInfo(line,&processNum,&threadCounter);
				if (threadCounter > 0){
					state = threadprops;
				}
				break;

			case threadprops:

				curThread = getThread(line,processNum);
				burstNum = curThread->burstCount;
				push(sim->threads,curThread);

				if (curThread->burstCount > 0){
					/*Create burst list*/
					if (!curThread->burst){
						createList(&(curThread->burst));
					}
				}

				/*Get next bursts properties*/
				if (burstNum > 0){
					state = burstprops;
				}

				break;

			case burstprops:

				/*Get next burst*/
				curBurst = getBurst(line);

				/*Add burst to queue*/
				push(curThread->burst,curBurst);

				burstNum--;
				if (burstNum == 0){
					state = threadprops;	
					threadCounter--;
				}

				break;
		}

		free(line);

	}while (!finished);
 
	return sim;
}

static void printSimulationData(Simulation *sim){
	Thread *thread;
	Node *procNext;
	Node *threadNext;
	Node *burstNext;

	printf("-----Simulation Properties-----\n");
	printf("processCount:%d\tthreadCount:%d\tsameSwitchTm:%d\tdiffSwitchTm:%d\n"
		,sim->processCount,sim->threads->count
		,sim->sameSwitchTm,sim->diffSwitchTm);

	/*Traverse threads and print each one*/
	threadNext = getHead(sim->threads);
	if (!threadNext){ return; }

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

	printf("----------------------------------\n");
}

static void freeThreads(void *data){
	Thread *thread = (Thread*)data;

	/*Free threads burst list*/
	if (thread->burst){
		freeHard(&thread->burst,free);
	}
	/*Free node data(process)*/
	free(data);
}

static void freeSimulationRes(SimulationRes **sim){
	if ((*sim)->threads){
		freeThreads(&((*sim)->threads));
	}
	if ((*sim)->stateEvents){
		freeHard(&((*sim)->stateEvents),free);
	}
	free(*sim);
	*sim = NULL;
}

static void freeSimulation(Simulation **sim){
	if ((*sim)->threads){
		freeHard(&((*sim)->threads),freeThreads);
	}
	free(*sim);
	*sim = NULL;
}

int threadCmp(const void *a,const void *b){
	Thread *thread1 = *((Thread**)a);
	Thread *thread2 = *((Thread**)b);
	return (thread1->arrivalTm - thread2->arrivalTm);
}

static void sortThreads(List *threads){
	int threadNum;
	Node *node=NULL;
	Thread *curThread=NULL;
	Thread **threadarr=NULL;

	threadNum = threads->count;
	threadarr = malloc(sizeof(Thread*)*threadNum);

	/*Add threads from all processes to array*/
	for (int i=1;i<=threadNum;i++){
		node = getNode(threads,i);
		threadarr[i-1] = getData(node);
	}

	/*Empty list to allow for restorage*/
	for (int i=1;i<=threadNum;i++){
		removeSoft(threads,1);
	}
	
	/*Sort the array of thread nodes*/
	qsort(threadarr,threadNum,sizeof(Thread*),threadCmp);

	/*Push sorted thread nodes into queue*/
	for (int i=0;i<threadNum;i++){
		push(threads,threadarr[i]);
	}

	free(threadarr);
}

static void printThreadRes(void *data){
	Thread *thread = (Thread*)data;

	printf("Thread %d of Process %d:\n"
		,thread->threadNum,thread->processNum );
	printf(" arrival time: %d\n",thread->arrivalTm);
	printf(" serivce time: %d units, I/O time: %d units"
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

	printf("At time %d: Thread %d of Process %d moves from "
		,event->time,event->threadNum,event->processNum);
	printSimState(event->from);
	printf(" to ");
	printSimState(event->to);
	printf("\n");
}

static void printResults(SimulationRes *res
	, bool detInfo, bool verbMode, int timeQuant){

	if (timeQuant == 0){
		printf("\nFCFS Scheduling\n");
	}
	else{
		printf("Round Robin Scheduling(quantum = %d time units)\n"
			,timeQuant);
	}
	printf("Total Time required is: %d units\n",res->totalTm);
	printf("Average Turnaround Time is %.2f time units\n"
		,res->avgTurnTm);
	printf("CPU Utilization is %d%%\n",res->cpuUtil);

	if (detInfo){
		printf("\n");
		printList(*res->threads,printThreadRes);
	}
	if (verbMode){
		printf("\n");
		printList(*res->stateEvents,printStateEvent);
	}
}


static void initSimulationRes(SimulationRes *res){
	res->totalTm = 0;
	res->avgTurnTm = 0;
	res->cpuUtil = 0;
	res->stateEvents = NULL;
	res->threads = NULL;
}

static void 

static SimulationRes *runSim(Simulation *data){
	int curTime,prevProcNum,curProcNum,switchCounter=0;
	int noUtil=0;
	bool switched=false;
	List *waitingList=NULL;
	List *readyList=NULL;
	List *eventList=NULL;
	List *curBurstList;
	Burst *curBurst;
	Thread *curThread,*checkThread;
	SimulationRes *res;

	res = malloc(sizeof(SimulationRes));
	initSimulationRes(res);

	/*Initialize lists*/
	createList(&waitingList);
	eventList = data->threads;


	/*Simulate while threads not done running*/
	while (!empty(eventList) || !empty(waitingList)){

		/*Add events that made it to the ready queue
		from the previous turn. This is being done here
		to prevent a thread from getting an I/O turn
		and a cpu turn in the same time unit*/

		/*Simulate I/O*/


		/*Simulate events if any exist*/
		if (!empty(eventList)){

			/*Get top of event queue*/
			curThread = getData(getHead(eventList));
			curBurstList = curThread->burst;
			curBurst = getData(getHead(curBurstList));

			/*Check whether current burst is done but make
			sure no context switch is occurring*/
			if (curBurst->cpuTm == 0 && !switched){

				/*Check if I/O is needed*/
				if (curBurst->IOTm > 0){
					/*Add current thread waiting list
					and remove from events queue*/
					prevProcNum = curThread->processNum;
					push(waitingList,pop(eventList));
					switched = true;

					/*Check if switched same proc or different*/
					if (!empty(eventList)){
						checkThread = getTop(eventList);
						curProcNum = checkThread->processNum;
						if (curProcNum != prevProcNum){
							switchCounter = data->diffSwitchTm;
						}
						else{
							switchCounter = data->sameSwitchTm;
						}
					}
				}

				/*Remove burst*/
				else{
					pop(curBurstList);
					/*Move to next thread if done*/
					if (curBurstList->count == 0){
						prevProcNum = curThread->processNum;
						pop(eventList);
						switched = true;
					

						/*Check if switched same proc or different*/
						if (!empty(eventList)){
							checkThread = getTop(eventList);
							curProcNum = checkThread->processNum;
							if (curProcNum != prevProcNum){
								switchCounter = data->diffSwitchTm;
							}
							else{
								switchCounter = data->sameSwitchTm;
							}
						}
					}
				}
			}
		}
		/*If events are empty than cpu not being utilized*/
		else{
			noUtil++;
		}

		/*Context switch is occurring*/
		if (switched){
			switchCounter--;
			if (switchCounter == 0){
				switched = false;
			}
		}
		else{
			/*Simulate one cpu time*/
			curBurst->cpuTm--;
		}
		curTime++;
	}
}

int main (int argc, char **argv){
	int timeQuant=0;
	bool detInfo=false,verbMode=false;
	Simulation *sim;
	SimulationRes *res;

	for (int i=0;i<argc;i++){
		if (!strcmp(argv[i],"-d")){
			detInfo = true;
		}
		else if (!strcmp(argv[i],"-v")){
			verbMode = true;
		}
		else if (!strcmp(argv[i],"-r")){
			if (argv[i+1]){
				timeQuant = strtol(argv[i+1],NULL,10);
			}
			else{
				fprintf(stderr,"Error: round robin enabled but time quantum not specified.\n");
				return(EXIT_FAILURE);
			}
		}
	}

	/*Get data ready for simulation*/
	sim = fillSimProps();
	sortThreads(sim->threads);

	/*Run simulation and print results*/
	res = runSim(sim);
	printResults(res,detInfo,verbMode,timeQuant);

	/*Free memory*/
	freeSimulation(&sim);
	freeSimulationRes(&res);
	return 0;
}